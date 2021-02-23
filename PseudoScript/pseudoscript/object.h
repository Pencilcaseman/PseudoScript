#pragma once

#include <cstring>
#include <cstdint>

#pragma warning(disable : 4996)

#define OBJECT Object *
#define INT(x) int64_t(x)
#define FLOAT(x) double(x)
#define STRING(x) ((const char *) (x))

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

#define PTR_CAST(ret, val) (ret) (val)
#define PTR_EVAL(type, val) ((type) (val))

inline OBJECT COPY(OBJECT val);

template<typename t>
inline OBJECT CREATE_OBJECT(const char *type, const std::initializer_list<OBJECT> &data, t value)
{
	// auto newValue = new t(value);
	auto newValue = (t *) malloc(sizeof(t));
	*newValue = value;

	if (data.size() != 0)
	{
		// auto start = omp_get_wtime() * 1000000000;
		uint64_t index = 0;
		// OBJECT *members = new OBJECT[data.size()];
		OBJECT *members = (OBJECT *) malloc(sizeof(OBJECT) * data.size());

		// auto end = omp_get_wtime() * 1000000000;

		for (auto &val : data)
		{
			INC_REF(val);
			members[index] = val; // COPY(val);
			index++;
		}

		// std::cout << "Time: " << end - start << "\n";

		// return new Object{type, index, members, true, PTR_CAST(void *, (uint64_t *) newValue), new uint64_t(1)};
		OBJECT res = (OBJECT) malloc(sizeof(Object));
		auto references = (uint64_t *) malloc(sizeof(uint64_t));
		*references = 1;
		*res = Object{type, index, members, true, PTR_CAST(void *, (uint64_t *) newValue), references};
		return res;
	}
	
	// return new Object{type, 0, nullptr, true, PTR_CAST(void *, (uint64_t *) newValue), new uint64_t(1)};
	OBJECT res = (OBJECT) malloc(sizeof(Object));
	auto references = (uint64_t *) malloc(sizeof(uint64_t));
	*references = 1;
	*res = Object{type, 0, nullptr, true, PTR_CAST(void *, (uint64_t *) newValue), references};
	return res;
};

template<typename t>
inline OBJECT CREATE_OBJECT(const char *type, t value)
{
	// auto newValue = new t(value);
	auto newValue = (t *) malloc(sizeof(t));
	*newValue = value;
	
	// return new Object{type, 0, nullptr, true, PTR_CAST(void *, (uint64_t *) newValue), new uint64_t(1)};
	OBJECT res = (OBJECT) malloc(sizeof(Object));
	auto references = (uint64_t *) malloc(sizeof(uint64_t));
	*references = 1;
	*res = Object{type, 0, nullptr, true, PTR_CAST(void *, (uint64_t *) newValue), references};
	return res;
};

template<>
inline OBJECT CREATE_OBJECT(const char *type, const char *value)
{
	// auto newValue = new char[strlen(value) + 1]();
	auto newValue = (char *) malloc(sizeof(char) * (strlen(value) + 1));
	strcpy(newValue, value);
	
	// return new Object{type, 0, nullptr, true, PTR_CAST(void *, (uint64_t *) newValue), new uint64_t(1)};

	OBJECT res = (OBJECT) malloc(sizeof(Object));
	auto references = (uint64_t *) malloc(sizeof(uint64_t));
	*references = 1;
	*res = Object{type, 0, nullptr, true, PTR_CAST(void *, (uint64_t *) newValue), references};
	return res;
};

template<>
inline OBJECT CREATE_OBJECT(const char *type, char *value)
{
	// auto newValue = new char[strlen(value) + 1]();
	auto newValue = (char *) malloc(sizeof(char) * (strlen(value) + 1));
	strcpy(newValue, value);
	
	// return new Object{type, 0, nullptr, true, PTR_CAST(void *, (uint64_t *) newValue), new uint64_t(1)};
	OBJECT res = (OBJECT) malloc(sizeof(Object));
	auto references = (uint64_t *) malloc(sizeof(uint64_t));
	*references = 1;
	*res = Object{type, 0, nullptr, true, PTR_CAST(void *, (uint64_t *) newValue), references};
	return res;
};

inline void DESTROY_OBJECT(OBJECT object)
{
	DEC_REF(object);
	if (*(object->references) == 0)
	{
		if (object->shouldFreeValue)
			if (object->type == "string")
			{
				char *value = PTR_EVAL(char *, (uint64_t *) object->value);
				// delete value;
				free(value);
			}
			else
				// delete object->value;
				free(object->value);

		for (uint64_t i = 0; i < object->memberCount; i++)
			DESTROY_OBJECT(object->members[i]);
		if (object->memberCount != 0)
			// delete[] object->members;
			free(object->members);

		// delete object->references;
		// delete object;
		free(object->references);
		free(object);
	}
}

#define SET_ATTRIBUTE(obj, attr, val) ((obj->attr = val))
#define GET_ATTRIBUTE(obj, attr) (obj->attr)

template<typename t>
inline void SET_VALUE(OBJECT obj, t val, const char *type = "NONE")
{
	if (obj->shouldFreeValue)
		// delete obj->value;
		free(obj->value);

	// auto newValue = new t(val);
	auto newValue = (t *) malloc(sizeof(t));
	*newValue = val;

	obj->value = PTR_CAST(void *, (uint64_t *) newValue);
	obj->type = type;
};

#define GET_VALUE(obj, type, stored) (PTR_EVAL(type, (stored) obj->value))

#define GET_TYPE(obj) (GET_ATTRIBUTE(obj, type))

#define CREATE_INT(val) CREATE_OBJECT("int", INT(val))
#define CREATE_FLOAT(val) CREATE_OBJECT("float", FLOAT(val))
#define CREATE_STRING(val) CREATE_OBJECT("string", STRING(val))

inline OBJECT COPY(OBJECT obj)
{
	// auto newValue = new uint64_t(*PTR_CAST(uint64_t *, obj->value));

	auto newValue = (uint64_t *) malloc(sizeof(uint64_t));
	*newValue = *PTR_CAST(uint64_t *, obj->value);

	if (obj->memberCount != 0)
	{
		// OBJECT *members = new OBJECT[obj->memberCount];
		auto members = (OBJECT *) malloc(sizeof(OBJECT) * obj->memberCount);

		for (uint64_t i = 0; i < obj->memberCount; i++)
			members[i] = COPY(obj->members[i]);
		
		// return new Object{obj->type, obj->memberCount, members, obj->shouldFreeValue, PTR_CAST(void *, (uint64_t *) newValue), new uint64_t(1)};
		auto res = (OBJECT) malloc(sizeof(Object));
		auto references = (uint64_t *) malloc(sizeof(uint64_t));
		*references = 1;
		*res = Object{obj->type, obj->memberCount, members, obj->shouldFreeValue, PTR_CAST(void *, (uint64_t *) newValue), references};
		return res;
	}
	
	// return new Object{obj->type, obj->memberCount, nullptr, obj->shouldFreeValue, PTR_CAST(void *, (uint64_t *) newValue), new uint64_t(1)};
	auto res = (OBJECT) malloc(sizeof(Object));
	auto references = (uint64_t *) malloc(sizeof(uint64_t));
	*references = 1;
	*res = Object{obj->type, obj->memberCount, nullptr, obj->shouldFreeValue, PTR_CAST(void *, (uint64_t *) newValue), references};
	return res;
}

#include "objectFunctions.h"
