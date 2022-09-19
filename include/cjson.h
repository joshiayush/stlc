// Copyright 2021, The cjson authors.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of The cjson authors. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef CJSON_INCLUDE_CJSON_H_
#define CJSON_INCLUDE_CJSON_H_

#include <sys/types.h>

#include "bool.h"
#include "data/map/map.h"
#include "data/sstream/sstream.h"
#include "data/vector/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off
typedef void*     json_null_t;
typedef char*     json_string_t;
typedef bool_t    json_bool_t;
typedef int64_t   json_number_t;
typedef double    json_decimal_t;
typedef Vector    json_list_t;
typedef Map       json_object_t;
// clang-format on

#define JSON_STRINGIFY(o) ((json_string_t)o)
#define JSON_CONST_STRINGIFY(o) ((const json_string_t)o)
#define JSON_NUMBERIFY(o) ((json_number_t)o)
#define JSON_DECIMALIFY(o) ((json_decimal_t)o)
#define JSON_BOOLIFY(o) ((json_bool_t)o)
#define JSON_NULLIFY(o) ((json_null_t)o)
#define JSON_LISTIFY(o) ((json_list_t)o)
#define JSON_OBJECTIFY(o) ((json_object_t)o)

// `JSON_type` stores the different `JSON` types that we can possibly have.
//
// This `enum` is used to identify the type of the `JSON_value` instance
// currently holding in it.
typedef enum JSON_type {
  // clang-format off
  JSON_Null = 0, JSON_String = 1, JSON_Number = 2, JSON_Decimal = 3,
  JSON_Boolean = 4, JSON_List = 5, JSON_Object = 6
  // clang-format on
} JSON_type;

// `JSON_value` union stores `JSON` style values.  `union` is preferred over a
// `struct` to save memory.  Only one the value at a time can be stored while
// the others will be currupted.
typedef union JSON_value {
  // clang-format off
  json_null_t null; json_bool_t boolean; json_string_t string;
  json_number_t number; json_decimal_t decimal; json_list_t list;
  json_object_t object;
  // clang-format on
} JSON_value;

// Stores the `JSON` data and its type.  We can't infer the type of the `JSON`
// data we are holding in a `JSON` instance so we need to store it explicitly in
// a separate variable refer to as `type` which is a `JSON_type` enum.
//
// Values are stored inside of the `JSON_value` union container to save memory
// by only sharing a maximum common amount of memory where we can only perform
// read and write operations over a single value at a time.
typedef struct JSON {
  JSON_type type;
  JSON_value value;
} JSON;

// Returns a `JSON` instance of the given `type`.
//
// This function assumes that the requested `JSON` instance has a type size of
// `0` thus calls the function `JSON_TypeSize(type, 0)` with the given type
// and the `size` as `0`.
JSON JSON_InitType(const JSON_type type);

// Returns a `JSON` instance of the given `type`, in case `size` is given
// allocates `size` amount of memory in the free-store for the `JSON` instance.
//
// Uses the given `size` to allocate `size` bytes for either the `Vector`
// or the `Map` instance inside the `JSON_value` `union`.
JSON JSON_InitTypeSize(const JSON_type type, const size_t size);

#define JSON_INIT_TYPE(type) JSON_InitType(JSON_##type)
#define JSON_INIT_TYPE_SIZE(type, size) JSON_InitTypeSize(JSON_##type, size)

// Returns a `JSON` instance containing `null` value.
//
// Unlike the other allocation methods this method does not take in an explicit
// argument reason being the `JSON` instance automatically assigned a `null`
// value inside function `JSON_InitTypeSize()` as soon the `type` is inferred as
// `JSON_null`.
JSON JSON_InitNullImpl();

// Creates a `JSON` instance from a `json_string_t` type.
//
// Allocates free-store memory to store the comming `string` instance. Assigns
// `NULL` to the `json.value.string` instance if dynamic-memory allocation
// failed.
JSON JSON_InitStringImpl(const json_string_t string);

// Creates a `JSON` instance from a `json_number_t` type.
//
// The given number is copied to the `json.value.number` instance of the
// `JSON` instance and can be access as long as `JSON` instance is in the
// memory.
JSON JSON_InitNumberImpl(const json_number_t number);

// Creates a `JSON` instance from a `json_decimal_t` type.
//
// The given number is copied to the `json.value.decimal` instance of the
// `JSON` instance and can be access as long as `JSON` instance is in the
// memory.
//
// This particular function is aimed to take in `floating-point` values of
// type `double`.
JSON JSON_InitDecimalImpl(const json_decimal_t decimal);

// Creates a `JSON` instance from a `json_bool_t` type.
//
// The given number, which is a `json_bool_t` type which unwinds to
// `__uint8_t` can be either `0` ("false") or any value greater than and
// less than `256` to be stored as a `JSON_boolean` type data inside
// `json.value.boolean`.
JSON JSON_InitBoolImpl(const json_bool_t boolean);

// Creates a `JSON` instance from a `json_list_t*` pointer type.
//
// Saves a `json_list_t*` value by going over the `data` attribute of
// `Vector` instance and copying the addresses of the values the `data`
// linked list is pointing to.
//
// Note: This function does not copy the `void*` instances in the free-store
// instead only creates a new table in the free-store while keeping the pointer
// to the initial values regardless of their scope in the memory.  So if the
// initial data dies because it was allocated in stack not in the free-store
// then the `JSON` instance will lose that data too, so make sure to pass in
// dynamically allocated data.
JSON JSON_InitListImpl(json_list_t* list);

// Creates a `JSON` instance from a `json_object_t*` pointer type.
//
// Saves a `json_object_t*` value by going over the `buckets` attribute of
// the `Map` instance and copying the addresses of the values the `buckets`
// linked list is pointing to.
//
// Note: This function does not copy the `MapEntry` instances in the
// free-store instead only creates a new table in the free-store while keeping
// the pointer to the initial values regardless of their scope in the memory.
// So if the initial data dies because it was allocated in stack not in the
// free-store then the `JSON` instance will lose that data too, so make sure
// to pass in dynamically allocated data.
JSON JSON_InitObjectImpl(json_object_t* object);

// Returns a `JSON *` of the given `type`.
//
// This function assumes that the requested `JSON *` instance has a type size of
// `sizeof(JSON)` thus calls the function `JSON_AllocSize(type, sizeof(JSON))`
// with the given type and the `size` as `sizeof(JSON)`.
JSON* JSON_AllocType(JSON_type type);

// Returns a `JSON *` instance of the given `type`, in case `size` is given
// allocates `size` amount of memory in the free-store for the `JSON` instance.
//
// Uses the given `size` to allocate `size` bytes for either the `Vector` or the
// `Map` instance inside the `JSON_value` `union`.
//
// In case the size is zero, returns a `NULL` pointer or a unique-pointer
// returned by `malloc`.
JSON* JSON_AllocTypeSize(JSON_type type, size_t size);

#define JSON_ALLOC_TYPE(type) JSON_AllocType(JSON_##type)
#define JSON_ALLOC_TYPE_SIZE(type, size) JSON_AllocTypeSize(JSON_##type, size)

void JSON_Free(JSON* const json);
void JSON_FreeDeep(JSON* const json);

#define JSON_INIT(type) JSON_Init##type##Impl()
#define JSON_INIT_VAL(type, value) JSON_Init##type##Impl(value)

#ifdef __cplusplus
}
#endif

#include "accessors.h"
#include "modifiers.h"

#endif  // CJSON_INCLUDE_CJSON_H_
