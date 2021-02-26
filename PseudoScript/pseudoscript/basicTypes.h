#pragma once

#include "object.h"

/*
The base integer type. This is used to
represent all integral values, which are
stored as a 64bit signed integer.
*/
typedef struct
{
	OB_BASE;
	INT value;
} BasicInt;

// Allocate memory for an integral type
static Object *BasicInt_alloc(TypeObject *self, UINT nItems)
{
	return (Object *) OB_MALLOC(self->tp_size * (nItems == 0 ? 1 : nItems));
}

// Free memory for an integral type
static void BasicInt_free(void *self)
{
	OB_FREE(self);
}

// Deallocate an integral type
static void BasicInt_dealloc(Object *self)
{
	OB_TYPE(self)->tp_free(self);
}

// Allocate memory for an integral type
static Object *BasicInt_new(TypeObject *type, Object *args)
{
	BasicInt *self = (BasicInt *) type->tp_alloc(type, 0);

	if (self != nullptr)
		self->value = 0;

	return (Object *) self;
}

/*
Initialize an integral value. This does nothing because the
required argument is an integer type, which would require initializing
an integer from an integer type, which would require initializing
an integer type...
*/
static int BasicInt_init(BasicInt *self, Object *args)
{
	return 0;
}

Object *newInt(INT value);

/*
Create a new integral value. This is the only
way to create a new integer object
*/
static Object *BasicInt_get_value(BasicInt *self)
{
	return newInt(self->value);
}

/*
Getters and setters for the integral type
*/
static GetSet BasicInt_getset[] = {
	{"value", (getter) BasicInt_get_value, nullptr},
	{nullptr}
};

static TypeObject BasicInt_type = {
	"int",
	sizeof(BasicInt),
	BasicInt_new,
	(initFunc) BasicInt_init,
	BasicInt_dealloc,
	BasicInt_alloc,
	BasicInt_free,
	BasicInt_getset,
	nullptr,
	nullptr
};

Object *newInt(INT value)
{
	BasicInt *res = newObject(BasicInt, &BasicInt_type);
	res->value = value;
	return (Object *) res;
}

// ============================================================================================

typedef struct
{
	OB_BASE;
	FLOAT value;
} BasicFloat;

static Object *BasicFloat_alloc(TypeObject *self, UINT nItems)
{
	return (Object *) OB_MALLOC(self->tp_size * (nItems == 0 ? 1 : nItems));
}

static void BasicFloat_free(void *self)
{
	OB_FREE(self);
}

static void BasicFloat_dealloc(Object *self)
{
	OB_TYPE(self)->tp_free(self);
}

static Object *BasicFloat_new(TypeObject *type, Object *args)
{
	BasicFloat *self = (BasicFloat *) type->tp_alloc(type, 0);

	if (self != nullptr)
		self->value = 0;

	return (Object *) self;
}

static int BasicFloat_init(BasicFloat *self, Object *args)
{
	self->value = 3.1415;

	return 0;
}

Object *newFloat(FLOAT value);

static Object *BasicFloat_get_value(BasicFloat *self)
{
	return newFloat(self->value);
}

static GetSet BasicFloat_getset[] = {
	{"value", (getter) BasicFloat_get_value, nullptr},
	{nullptr}
};

static TypeObject BasicFloat_type = {
	"float",
	sizeof(BasicFloat),
	BasicFloat_new,
	(initFunc) BasicFloat_init,
	BasicFloat_dealloc,
	BasicFloat_alloc,
	BasicFloat_free,
	BasicFloat_getset,
	nullptr,
	nullptr
};

Object *newFloat(FLOAT value)
{
	BasicFloat *res = newObject(BasicFloat, &BasicFloat_type);
	res->value = value;
	return (Object *) res;
}
