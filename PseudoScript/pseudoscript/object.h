#pragma once

#include "allocator.h"

// Typedefs
typedef long long INT;
typedef unsigned long long UINT;
typedef double FLOAT;

typedef struct Object Object;
typedef struct TypeObject TypeObject;

/*
The base object type, storing the number
of references to the object and it's base type.

This object should never be created explicitly, but
casted to an (Object *)
*/
typedef struct Object
{
	UINT ob_ref;
	TypeObject *ob_type;
} Object;

typedef Object *(*getter)(Object *);
typedef int (*setter)(Object *, Object *);
typedef int(*initFunc)(Object *self, Object *args);
typedef Object *(*CFunction)(Object *);

/*
A type to store member/method getters and setters
*/
typedef struct
{
	const char *gs_name;
	Object *(*gs_get)(Object *self);
	int (*gs_set)(Object *self, Object *value);
} GetSet;

#define T_INT 0;
#define T_FLOAT 1;
#define T_DOUBLE 2;
#define T_STRING 3;

/*
Defines a member of a class or object, storing the name,
type, and value to be contained
*/
typedef struct
{
	const char *mm_name;
	UINT mm_type;
	void *mm_value;
} MemberDef;

/*
Defines a method of a class, storing the name and a
pointer to the C implementation
*/
typedef struct
{
	const char *mt_name;
	Object *(*mt_meth)(Object *);
} MethodDef;

/*
Used to store any type information, such as members, methods
name, size, built-in functions, etc.
*/
typedef struct TypeObject
{
	const char *tp_name;                                   // Name of type
	UINT tp_size;                                          // Size of the object in bytes
	Object *(*tp_new)(TypeObject *type, Object *args);     // Allocate a new object and return a pointer to it
	int (*tp_init)(Object *self, Object *args);            // Initialize an object
	void (*tp_dealloc)(Object *self);                      // Free an object that has been created
	Object *(*tp_alloc)(TypeObject *self, UINT nItems);    // Allocate memory for an object of this type
	void (*tp_free)(void *obj);                            // Free an object of a given type
	GetSet *tp_getset;                                     // Getters and setters
	MemberDef *members;                                    // Member definitions for the type
	MethodDef *methods;                                    // Method definitions for the type
} TypeObject;

#define OB_BASE Object ob_base
#define OB_TYPE(x) (((Object *) (x))->ob_type)

Object *_new_object(TypeObject *type)
{
	Object *res = (Object *) OB_MALLOC(type->tp_size);
	res->ob_ref = 0;
	res->ob_type = type;
	return res;
}

#define newObject(object, type) ((object *) _new_object(type))
