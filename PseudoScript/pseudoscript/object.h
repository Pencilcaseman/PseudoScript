#pragma once

#include <cstring>
#include <cstdint>

#define OBJECT Object *

typedef struct Object Object;

typedef struct Object
{
	const char *type;
	unsigned long long memberCount;
	Object **members;
	bool shouldFreeValue;
	void *value;
	uint64_t *references;
} Object;

#define INC_REF(obj) (*obj->references)++
#define DEC_REF(obj) (*obj->references)--

#define PTR_CAST(ret, val) (ret *) (val)
#define PTR_EVAL(type, val) *(type *) (val)

template<typename t>
inline Object *CREATE_OBJECT(const char *type, std::initializer_list<OBJECT> data, bool shouldFree, t value)
{
	if (data.size() != 0)
	{
		uint64_t index = 0;
		OBJECT *members = new OBJECT[data.size()];
		for (auto &val : data)
		{
			INC_REF(val);
			members[index++] = val;
		}
		return new Object{type, index, members, shouldFree, PTR_CAST(void, (uint64_t *) (value)), new uint64_t(1)};
	}
	return new Object{type, 0, nullptr, shouldFree, PTR_CAST(void, (uint64_t *) (value)), new uint64_t(1)};
};

template<typename t>
inline Object *CREATE_OBJECT(const char *type, bool shouldFree, const t &value)
{
	return new Object{type, 0, nullptr, shouldFree, PTR_CAST(void, (uint64_t *) (&value)), new uint64_t(1)};
};

inline void DESTROY_OBJECT(OBJECT object)
{
	DEC_REF(object);
	if (*(object->references) == 0)
	{
		if (object->shouldFreeValue)
			delete object->value;
		for (uint64_t i = 0; i < object->memberCount; i++)
			DESTROY_OBJECT(object->members[i]);
		if (object->memberCount != 0)
			delete[] object->members;
		delete object->references;
		delete object;
	}
}

/*
const char *x = "Hello, World!";
auto val = PTR_CAST(uint64_t, &x);

std::cout << x << "\n";
std::cout << val << "\n";
std::cout << PTR_EVAL(const char *, val) << "\n";
*/

#define SET_ATTRIBUTE(obj, attr, val) ((obj->attr = val))
#define GET_ATTRIBUTE(obj, attr) (obj->attr)
#define SET_VALUE(obj, val, type) {if (obj->shouldFreeValue) {delete obj->value;}} (SET_ATTRIBUTE(obj, value, PTR_CAST(void, (type *) (val))))
#define GET_VALUE(obj, type, stored) (PTR_EVAL(type, (stored *) obj->value))

#define CREATE_INT(val) CREATE_OBJECT("int", false, val)
#define CREATE_FLOAT(val) CREATE_OBJECT("float", false, val)

#include "objectFunctions.h"
