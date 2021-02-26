#define PROGRAM_MEM_SIZE 4096   // Memory buffer size for the program. Bigger = more ram
#define CUSTOM_ALLOCATOR        // Disable this to use normal C++ allocations

#include <iostream>
#include "pseudoscript/pseudoscript.h"

int main()
{
	objectAllocator = newAllocator(PROGRAM_MEM_SIZE);

	printf("PseudoScript Testing\n");

	printf("Creating object\n");

	Object *myInt = newInt(123);
	Object *myFloat = newFloat(123);
	printf("Object created\n");

	OB_TYPE(myInt)->tp_dealloc(myInt);
	OB_TYPE(myFloat)->tp_dealloc(myFloat);

	UINT loops = 7140000; // 7140000;
	double start = TIME * 1000000000;
	for (UINT i = 0; i < loops; i++)
	{
		Object *a = newInt(123);
		Object *b = newFloat(3.1415);

		Object *c = newInt(123);
		Object *d = newFloat(3.1415);
		Object *e = newInt(99999);
		Object *f = newInt(99999);

		Object *val = (OB_TYPE(a)->tp_getset->gs_get(a));
		Object *val2 = (OB_TYPE(b)->tp_getset->gs_get(b));

		OB_TYPE(a)->tp_dealloc(a);
		OB_TYPE(val)->tp_dealloc(val);

		OB_TYPE(b)->tp_dealloc(b);
		OB_TYPE(val2)->tp_dealloc(val2);

		OB_TYPE(c)->tp_dealloc(c);
		OB_TYPE(d)->tp_dealloc(d);
		OB_TYPE(d)->tp_dealloc(e);
		OB_TYPE(f)->tp_dealloc(f);
	}
	double end = TIME * 1000000000;
	printf("Time: %.*fns\n", 10, (end - start) / (double) loops);

	printf("\n\n\n");

	freeAllocator(objectAllocator);

	Allocator *alloc = newAllocator(sizeof(int) * 4);

	auto data1 = (int *) requestMemory(alloc, sizeof(int) * 3); // [2]
	auto data2 = (int *) requestMemory(alloc, sizeof(int) * 1); // [1]

	data1[0] = 1;
	data1[1] = 2;
	data2[0] = 3;

	freeMemory(alloc, data1);

	auto data3 = (int *) requestMemory(alloc, sizeof(int) * 1); // [1, 1]
	auto data4 = (int *) requestMemory(alloc, sizeof(int) * 1); // ^^^^^^

	data3[0] = 4;
	data4[0] = 5;

	if (data1 == nullptr)
		printf("Data 1 is NULL\n");

	if (data2 == nullptr)
		printf("Data 1 is NULL\n");

	if (data3 == nullptr)
		printf("Data 1 is NULL\n");

	if (data4 == nullptr)
		printf("Data 1 is NULL\n");

	printf("Data1[0] | 4  : %i\n", data1[0]);
	printf("Data1[1] | 5  : %i\n", data1[1]);
	printf("Data2[0] | 3  : %i\n", data2[0]);
	printf("Data3[0] | 4  : %i\n", data3[0]);
	printf("Data4[0] | 5  : %i\n", data4[0]);

	freeAllocator(alloc);

	return 0;
}
