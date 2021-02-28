#pragma once

#include <cstdio>
#include "object.h"

// Important function forward-declares
Object *newInt(INT value);
Object *newFloat(FLOAT value);
Object *newString(STRING value);

// ====================================================================================

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
Getters and setters for the integral type
*/
static GetSet BasicInt_getset[] = {
	{nullptr}
};

/*
Access the exact value stored
*/
static Object *BasicInt_value(BasicInt *self)
{
	return newInt(self->value);
}

/*
Convert to string
*/
static Object *BasicInt_toString(BasicInt *self)
{
	auto res = (char *) OB_MALLOC(sizeof(char) * ((UINT) ceil(log10(self->value)) + 1));
#pragma warning(suppress : 4996)
	sprintf(res, "%i", (int) self->value);
	return newString(res);
}

static TypeObject BasicInt_type = {
	"int",                         // Name of type
	sizeof(BasicInt),              // Size of the object in bytes
	BasicInt_new,                  // Allocate a new object and return a pointer to it
	(initFunc) BasicInt_init,      // Initialize an object
	BasicInt_dealloc,              // Free an object that has been created
	BasicInt_alloc,                // Allocate memory for an object of this type
	BasicInt_free,                 // Free an object of a given type
	BasicInt_getset,               // Getters and setters
	(CFunction) BasicInt_value,    // Return the EXACT value stored -- i.e. \"Hello, World!\"
	(CFunction) BasicInt_toString  // Return a string representation of the value
	// Return an integer representation of the value
	// Return a floating point representation of the value
	// Return the result of addition
	// Return the result of subtraction
	// Return the result of multiplication
	// Return the result of division
	// Return the result of raising to the power of a value
	// Return the result of reverse addition -- see notes ^^^
	// Return the result of reverse subtraction -- see notes ^^^
	// Return the result of reverse multiplication -- see notes ^^^
	// Return the result of reverse division -- see notes ^^^
	// Return the result of the reverse power -- see notes ^^^
	// Member definitions for the type
	// Method definitions for the type
};

Object *newInt(INT value)
{
	BasicInt *res = newObject(BasicInt, &BasicInt_type);
	res->value = value;
	return (Object *) res;
}

#ifdef PS_DEBUG
INT OB_INT_TO_C(Object *x)
{
	if (OB_TYPE(x)->tp_name != "int")
		throw std::logic_error("Invalid variable type to cast to C INT");
	return ((*((BasicInt *) x)).value);
}
#else
#define OB_INT_TO_C(x) ((*((BasicInt *) x)).value)
#endif

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
	"float",                    // Name of type
	sizeof(BasicFloat),         // Size of the object in bytes
	BasicFloat_new,             // Allocate a new object and return a pointer to it
	(initFunc) BasicFloat_init, // Initialize an object
	BasicFloat_dealloc,         // Free an object that has been created
	BasicFloat_alloc,           // Allocate memory for an object of this type
	BasicFloat_free,            // Free an object of a given type
	BasicFloat_getset,          // Getters and setters
	nullptr,                    // Return the EXACT value stored -- i.e. \"Hello, World!\"
	nullptr                     // Return a string representation of the value
	// Return an integer representation of the value
	// Return a floating point representation of the value
	// Return the result of addition
	// Return the result of subtraction
	// Return the result of multiplication
	// Return the result of division
	// Return the result of raising to the power of a value
	// Return the result of reverse addition -- see notes ^^^
	// Return the result of reverse subtraction -- see notes ^^^
	// Return the result of reverse multiplication -- see notes ^^^
	// Return the result of reverse division -- see notes ^^^
	// Return the result of the reverse power -- see notes ^^^
	// Member definitions for the type
	// Method definitions for the type
};

Object *newFloat(FLOAT value)
{
	BasicFloat *res = newObject(BasicFloat, &BasicFloat_type);
	res->value = value;
	return (Object *) res;
}

#ifdef PS_DEBUG
double OB_FLOAT_TO_C(Object *x)
{
	if (OB_TYPE(x)->tp_name != "float")
		throw std::logic_error("Invalid variable type to cast to C FLOAT");
	return ((*((BasicFloat *) x)).value);
}
#else
#define OB_FLOAT_TO_C(x) ((*((BasicFloat *) x)).value)
#endif

// ===========================================================================================

typedef struct
{
	OB_BASE;
	STRING value;
} BasicString;

static Object *BasicString_alloc(TypeObject *self, UINT nItems)
{
	return (Object *) OB_MALLOC(self->tp_size * (nItems == 0 ? 1 : nItems));
}

static void BasicString_free(void *self)
{
	OB_FREE(self);
}

static void BasicString_dealloc(Object *self)
{
	OB_TYPE(self)->tp_free(self);
}

static Object *BasicString_new(TypeObject *type, Object *args)
{
	auto self = (BasicString *) type->tp_alloc(type, 0);

	if (self != nullptr)
		self->value = "";

	return (Object *) self;
}

static int BasicString_init(BasicInt *self, Object *args)
{
	return 0;
}

static GetSet BasicString_getset[] = {
	{nullptr}
};

static Object *BasicString_value(BasicString *self)
{
	auto resStr = (char *) OB_MALLOC(sizeof(char) * (strlen(self->value) + 2));
#pragma warning(suppress : 4996)
	strcat(resStr, "\"");
#pragma warning(suppress : 4996)
	strcat(resStr, self->value);
#pragma warning(suppress : 4996)
	strcat(resStr, "\"");
	return newString(resStr);
}

static Object *BasicString_toString(BasicString *self)
{
	return newString(self->value);
}

static TypeObject BasicString_type = {
	"string",                      // Name of type
	sizeof(BasicString),              // Size of the object in bytes
	BasicString_new,                  // Allocate a new object and return a pointer to it
	(initFunc) BasicString_init,      // Initialize an object
	BasicString_dealloc,              // Free an object that has been created
	BasicString_alloc,                // Allocate memory for an object of this type
	BasicString_free,                 // Free an object of a given type
	BasicString_getset,               // Getters and setters
	(CFunction) BasicString_value,    // Return the EXACT value stored -- i.e. \"Hello, World!\"
	(CFunction) BasicString_toString  // Return a string representation of the value
	// Return an integer representation of the value
	// Return a floating point representation of the value
	// Return the result of addition
	// Return the result of subtraction
	// Return the result of multiplication
	// Return the result of division
	// Return the result of raising to the power of a value
	// Return the result of reverse addition -- see notes ^^^
	// Return the result of reverse subtraction -- see notes ^^^
	// Return the result of reverse multiplication -- see notes ^^^
	// Return the result of reverse division -- see notes ^^^
	// Return the result of the reverse power -- see notes ^^^
	// Member definitions for the type
	// Method definitions for the type
};

Object *newString(STRING value)
{
	auto res = newObject(BasicString, &BasicString_type);
	res->value = value;
	return (Object *) res;
}

#ifdef PS_DEBUG
STRING OB_STRING_TO_C(Object *x)
{
	if (OB_TYPE(x)->tp_name != "string")
		throw std::logic_error("Invalid variable type to cast to C STRING");
	return ((*((BasicString *) x)).value);
}
#else
#define OB_STRING_TO_C(x) ((*((BasicString *) x)).value)
#endif
