#pragma once

#include <cstdlib>
#include <omp.h>
#include <chrono>

#ifdef _OPENMP
#define TIME omp_get_wtime()
#else
#define TIME (std::chrono::high_resolution_clock().now().time_since_epoch().count() * 1000000000)
#endif

typedef struct Allocator Allocator;

// Memory location storage
typedef struct
{
	unsigned long long size;
	unsigned long long offset;
	int used;
} MemLoc;

// The allocator type
typedef struct Allocator
{
	unsigned long long bytes;
	unsigned long long used;
	MemLoc *memSpace;
	char *data;
	Allocator *next;
} Allocator;

// Create a new allocator
Allocator *newAllocator(unsigned long long bytes)
{
	Allocator *res = (Allocator *) malloc(sizeof(Allocator));

	res->bytes = bytes;
	res->used = 0;

	res->memSpace = (MemLoc *) malloc(sizeof(MemLoc) * bytes);
	for (unsigned long long i = 0; i < bytes; i++)
	{
		res->memSpace[i].size = 0;
		res->memSpace[i].offset = -1;
		res->memSpace[i].used = 0;
	}

	res->memSpace[0].size = bytes;
	res->memSpace[0].offset = 0;
	res->next = nullptr;

	res->data = (char *) malloc(bytes);

	return res;
}

// Request memory from an allocator object
void *requestMemory(Allocator *alloc, unsigned long long bytes)
{
	// If there isn't room, just create a new allocator
	if (alloc->bytes - alloc->used < bytes)
		goto skip;

	// Traverse memory space and search for a valid space
	for (unsigned long long i = 0; i < alloc->bytes - 1; i++)
	{
		// Check for a valid block
		if (!alloc->memSpace[i].used && alloc->memSpace[i].size >= bytes)
		{
			// Update variables
			alloc->used += bytes;

			// Partition the memory space
			if (!alloc->memSpace[i + 1].used)
			{
				alloc->memSpace[i + 1].size = alloc->bytes - alloc->used;
				alloc->memSpace[i + 1].offset = alloc->memSpace[i].offset + bytes;
				alloc->memSpace[i + 1].used = 0;
			}

			alloc->memSpace[i].size = bytes;
			alloc->memSpace[i].used = 1;

			return alloc->data + alloc->memSpace[i].offset;
		}
	}

skip:

	if (alloc->next == nullptr)
	{
		// Create a new allocator in a linked-list structure
		// Double the size to allow for large allocations that wouldn't normally fit
		alloc->next = newAllocator(alloc->bytes * 2);
		return requestMemory(alloc->next, bytes);
	}

	return requestMemory(alloc->next, bytes);
}

// Free memory from an allocator
void freeMemory(Allocator *alloc, void *data)
{
	unsigned long long offset = (char *) data - alloc->data;

	// If the offset is outside of the range of the allocator,
	// check the child allocator
	if (offset > alloc->bytes)
	{
		freeMemory(alloc->next, data);
		return;
	}

	for (unsigned long long i = 0; i < alloc->bytes; i++)
	{
		if (offset == alloc->memSpace[i].offset)
		{
			alloc->memSpace[i].used = 0;
			alloc->used -= alloc->memSpace[i].size;

			return;
		}
	}
}

// Free an allocator object
void freeAllocator(Allocator *alloc)
{
	if (alloc->next != nullptr)
		freeAllocator(alloc->next);

	free(alloc->memSpace);
	free(alloc->data);
	free(alloc);
}

Allocator *objectAllocator;

#ifdef CUSTOM_ALLOCATOR
#define OB_MALLOC(size) requestMemory(objectAllocator, size)
#define OB_FREE(data) freeMemory(objectAllocator, data)
#else
#define OB_MALLOC(size) malloc(size)
#define OB_FREE(data) free(data)
#endif
