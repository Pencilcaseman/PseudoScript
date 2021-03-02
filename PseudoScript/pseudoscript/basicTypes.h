#pragma once

#include <cstdio>
#include <cstdarg>
#include "object.h"

// TODO: Move this to a config file
constexpr UINT DECIMAL_POINTS = 10;

// Important function forward-declares
Object *newInt(INT value);
Object *newFloat(FLOAT value);
Object *newString(STRING value);
Object *newList_fromData(Object **data, UINT len);
Object *newList(UINT nargs, ...);

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
static void BasicInt_dealloc(BasicInt *self)
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

/*
Getters and setters for the integral type
*/
static GetSet BasicInt_getset[] = {
	{nullptr}
};

/*
Return a copy of the value stored
*/
static Object *BasicInt_copy(BasicInt *self)
{
	return newInt(self->value);
}

/*
Access the exact value stored
*/
static Object *BasicInt_represent(BasicInt *self)
{
	auto res = (char *) OB_MALLOC(sizeof(char) * ((UINT) ceil(log10(self->value)) + 1));
#pragma warning(suppress : 4996)
	sprintf(res, "%i", (int) self->value);
	return newString(res);
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
	"int",                           // Name of type
	sizeof(BasicInt),                // Size of the object in bytes
	BasicInt_new,                    // Allocate a new object and return a pointer to it
	(constructor) BasicInt_init,        // Initialize an object
	(destructor) BasicInt_dealloc,                // Free an object that has been created
	BasicInt_alloc,                  // Allocate memory for an object of this type
	BasicInt_free,                   // Free an object of a given type
	BasicInt_getset,                 // Getters and setters
	(CFunction) BasicInt_copy,       // Return an exact copy of a value
	(CFunction) BasicInt_represent,  // Return the value stored in a string form
	(CFunction) BasicInt_toString    // Return a string representation of the value
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

static void BasicFloat_dealloc(BasicFloat *self)
{
	OB_TYPE(self)->tp_free(self);
}

static Object *BasicFloat_new(TypeObject *type, Object *args)
{
	auto self = (BasicFloat *) type->tp_alloc(type, 0);

	if (self != nullptr)
		self->value = 0;

	return (Object *) self;
}

static int BasicFloat_init(BasicFloat *self, Object *args)
{
	return 0;
}

static Object *BasicFloat_copy(BasicFloat *self)
{
	return newFloat(self->value);
}

static GetSet BasicFloat_getset[] = {
	{nullptr}
};

static Object *BasicFloat_represent(BasicFloat *self)
{
	auto res = (char *) OB_MALLOC(sizeof(char) * ((UINT) ceil(log10((INT) self->value)) + DECIMAL_POINTS));
#pragma warning(suppress : 4996)
	sprintf(res, "%.*f", 10, self->value);
	return newString(res);
}

static Object *BasicFloat_toString(BasicFloat *self)
{
	auto res = (char *) OB_MALLOC(sizeof(char) * ((UINT) ceil(log10((INT) self->value)) + DECIMAL_POINTS));
#pragma warning(suppress : 4996)
	sprintf(res, "%.*f", DECIMAL_POINTS, self->value);
	return newString(res);
}

static TypeObject BasicFloat_type = {
	"float",                           // Name of type
	sizeof(BasicFloat),                // Size of the object in bytes
	BasicFloat_new,                    // Allocate a new object and return a pointer to it
	(constructor) BasicFloat_init,     // Initialize an object
	(destructor) BasicFloat_dealloc,   // Free an object that has been created
	BasicFloat_alloc,                  // Allocate memory for an object of this type
	BasicFloat_free,                   // Free an object of a given type
	BasicFloat_getset,                 // Getters and setters
	(CFunction) BasicFloat_copy,       // Return an exact copy of a value							
	(CFunction) BasicFloat_represent,  // Return the value stored in a string form
	(CFunction) BasicFloat_toString,   // Return a string representation of the value
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

static void BasicString_dealloc(BasicString *self)
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

static Object *BasicString_represent(BasicString *self)
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
	"string",                            // Name of type
	sizeof(BasicString),                 // Size of the object in bytes
	BasicString_new,                     // Allocate a new object and return a pointer to it
	(constructor) BasicString_init,      // Initialize an object
	(destructor) BasicString_dealloc,    // Free an object that has been created
	BasicString_alloc,                   // Allocate memory for an object of this type
	BasicString_free,                    // Free an object of a given type
	BasicString_getset,                  // Getters and setters
	(CFunction) BasicString_toString,    // Return an exact copy of a value
	(CFunction) BasicString_represent,   // Return the value stored in a string form
	(CFunction) BasicString_toString     // Return a string representation of the value
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

// ==============================================================================

typedef struct
{
	OB_BASE;
	Object **data;
	UINT len;
} BasicList;

static Object *BasicList_alloc(TypeObject *self, UINT nItems)
{
	return (Object *) OB_MALLOC(self->tp_size * (nItems == 0 ? 1 : nItems));
}

static void BasicList_free(void *self)
{
	OB_FREE(self);
}

static void BasicList_dealloc(BasicList *self)
{
	for (UINT i = 0; i < self->len; i++)
	{
		OB_TYPE(self->data[i])->tp_dealloc(self->data[i]);
	}

	OB_TYPE(self)->tp_free(self);
}

static Object *BasicList_new(TypeObject *type, Object *args)
{
	auto self = (BasicList *) type->tp_alloc(type, 0);

	if (self != nullptr)
	{
		self->data = nullptr;
		self->len = 0;
	}

	return (Object *) self;
}

static int BasicList_init(BasicList *self, Object *args)
{
	return 0;
}

static GetSet BasicList_getset[] = {
	{nullptr}
};

static Object *BasicList_copy(BasicList *self)
{
	auto resData = (Object **) OB_MALLOC(sizeof(Object *) * self->len);

	for (UINT i = 0; i < self->len; i++)
	{
		resData[i] = OB_TYPE(self->data[i])->tp_copy(self->data[i]);
	}

	return newList_fromData(resData, self->len);
}

static Object *BasicList_represent(BasicList *self)
{
	UINT minLen = 0;

	for (UINT i = 0; i < self->len; i++)
	{
		minLen += strlen(OB_STRING_TO_C(OB_TYPE(self->data[i])->tp_represent(self->data[i])));
	}

	UINT charLen = minLen + (2 * (self->len - 1)) + 2;

	auto chars = (char *) OB_MALLOC(sizeof(char) * charLen);

#pragma warning(suppress : 4996)
	strcat(chars, "[");

	for (UINT i = 0; i < self->len; i++)
	{
		if (self->data[i] == (Object *) self)
		{
		#pragma warning(suppress : 4996)
			strcat(chars, "...");
		}
		else
		{
		#pragma warning(suppress : 4996)
			strcat(chars, OB_STRING_TO_C(OB_TYPE(self->data[i])->tp_represent(self->data[i])));
		}

		if (i + 1 < self->len)
		{
		#pragma warning(suppress : 4996)
			strcat(chars, ", ");
		}
	}

#pragma warning(suppress : 4996)
	strcat(chars, "]");

	return newString(chars);
}

static Object *BasicList_toString(BasicList *self)
{
	return BasicList_represent(self);
}

static Object *BasicList_getItem(BasicList *self, Object *args)
{
	if (OB_TYPE(args)->tp_name != "int")
	{
		std::cout << "Indexing requires an integer value\n";
		exit(1);
	}

	auto index = OB_INT_TO_C(args);
	if (index < 0 || index >= self->len)
	{
		std::cout << "Index out of range for list getItem\n";
		exit(1);
	}

	return OB_TYPE(self->data[index])->tp_copy(self->data[index]);
}

static MethodDef BasicList_methods[] = {
	{"getItem", (pseudoFunc) BasicList_getItem},
	{nullptr}
};

static TypeObject BasicList_type = {
	"list",                          // Name of type
	sizeof(BasicList),               // Size of the object in bytes
	BasicList_new,                   // Allocate a new object and return a pointer to it
	(constructor) BasicList_init,    // Initialize an object
	(destructor) BasicList_dealloc,  // Free an object that has been created
	BasicList_alloc,                 // Allocate memory for an object of this type
	BasicList_free,                  // Free an object of a given type
	BasicList_getset,                // Getters and setters
	(CFunction) BasicList_copy,      // Return the EXACT value stored -- i.e. \"Hello, World!\"
	(CFunction) BasicList_represent, // Return the value stored in a string form
	(CFunction) BasicList_toString,   // Return a string representation of the value
	nullptr,                         // Return an integer representation of the value
	nullptr,                         // Return a floating point representation of the value
	nullptr,                         // Return the result of addition
	nullptr,                         // Return the result of subtraction
	nullptr,                         // Return the result of multiplication
	nullptr,                         // Return the result of division
	nullptr,                         // Return the result of raising to the power of a value
	nullptr,                         // Return the result of reverse addition -- see notes ^^^
	nullptr,                         // Return the result of reverse subtraction -- see notes ^^^
	nullptr,                         // Return the result of reverse multiplication -- see notes ^^^
	nullptr,                         // Return the result of reverse division -- see notes ^^^
	nullptr,                         // Return the result of the reverse power -- see notes ^^^
	nullptr,                         // Member definitions for the type
	BasicList_methods                // Method definitions for the type
};

Object *newList_fromData(Object **newData, UINT len)
{
	auto res = newObject(BasicList, &BasicList_type);

	res->data = newData;
	res->len = len;

	return (Object *) res;
}

Object *newList(UINT nargs, ...)
{
	auto res = newObject(BasicList, &BasicList_type);

	res->data = (Object **) OB_MALLOC(sizeof(Object *) * nargs);
	res->len = nargs;

	va_list elems;
	va_start(elems, nargs);
	for (UINT i = 0; i < nargs; i++)
	{
		auto value = va_arg(elems, Object *);
		res->data[i] = value;
	}
	va_end(elems);

	return (Object *) res;
}

#ifdef PS_DEBUG
Object *OB_LIST_TO_C(Object *x, UINT index)
{
	if (OB_TYPE(x)->tp_name != "list")
		throw std::logic_error("Invalid variable type to cast to C STRING");
	return ((BasicList *) x)->data[index];
}
#else
#define OB_LIST_TO_C(x, index) ((BasicList *) x)->data[index]
#endif
