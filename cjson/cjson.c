// Copyright 2021, joshiayush Inc.
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
//     * Neither the name of joshiayush Inc. nor the names of its
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

#include "cjson.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bool.h"
#include "data/sstream/sstream.h"
#include "data/vector/vector.h"

// Returns a ``JSON`` instance of the given ``type``.
//
// This function assumes that the requested ``JSON`` instance has a type size of
// ``0`` thus calls the function ``JSON_TypeSize(type, 0)`` with the given type
// and the ``size`` as ``0``.
JSON _JSONInitType(const JSON_type type) { return _JSONInitTypeSize(type, 0); }

// Returns a ``JSON`` instance of the given ``type``, in case ``size`` is given
// allocates ``size`` amount of memory in the free-store for the ``JSON``
// instance.
//
// Uses the given ``size`` to allocate ``size`` bytes for either the ``Vector``
// or the ``Map`` instance inside the ``JSON_value`` ``union``.
JSON _JSONInitTypeSize(const JSON_type type, const size_t size) {
  JSON json = {.type = type};
  switch (type) {
    case JSON_Null:
      json.value.null = NULL;
      break;
    case JSON_String:
      json.value.string = "";
      break;
    case JSON_Decimal:
      json.value.decimal = 0.0;
      break;
    case JSON_Number:
      json.value.number = 0;
      break;
    case JSON_Boolean:
      json.value.boolean = FALSE;
      break;
    case JSON_List:
      json.value.list = VectorAlloc(size);
      break;
    case JSON_Object:
      json.value.map = MapAllocNStrAsKey(size);
      break;
  }
  return json;
}

// Returns a ``JSON`` instance containing ``null`` value.
//
// Unlike the other allocation methods this method does not take in an explicit
// argument reason being the ``JSON`` instance automatically assigned a ``null``
// value inside function ``_JSONInitTypeSize()`` as soon the ``type`` is
// inferred as
// ``JSON_null``.
JSON _JSONInitNull() { return _JSONInitType(JSON_Null); }

// Creates a ``JSON`` instance from a ``json_string_t`` type.
//
// Allocates free-store memory to store the comming ``string`` instance. Assigns
// ``NULL`` to the ``json.value.string`` instance if dynamic-memory allocation
// failed.
JSON _JSONInitString(const json_string_t string) {
  JSON json = _JSONInitType(JSON_String);
  size_t strlen_ = strlen(string);
  if ((json.value.string = (char*)malloc((strlen_ + 1) * sizeof(char))) == NULL)
    return json;
  strcpy(json.value.string, string);
  return json;
}

// Creates a ``JSON`` instance from a ``json_number_t`` type.
//
// The given number is copied to the ``json.value.number`` instance of the
// ``JSON`` instance and can be access as long as ``JSON`` instance is in the
// memory.
JSON _JSONInitNumber(const json_number_t number) {
  JSON json = _JSONInitType(JSON_Number);
  json.value.number = number;
  return json;
}

// Creates a ``JSON`` instance from a ``json_decimal_t`` type.
//
// The given number is copied to the ``json.value.decimal`` instance of the
// ``JSON`` instance and can be access as long as ``JSON`` instance is in the
// memory.
//
// This particular function is aimed to take in ``floating-point`` values of
// type ``double``.
JSON _JSONInitDecimal(const json_decimal_t decimal) {
  JSON json = _JSONInitType(JSON_Decimal);
  json.value.decimal = decimal;
  return json;
}

// Creates a ``JSON`` instance from a ``json_bool_t`` type.
//
// The given number, which is a ``json_bool_t`` type which unwinds to
// ``__uint8_t`` can be either ``0`` ("false") or any value greater than and
// less than ``256`` to be stored as a ``JSON_boolean`` type data inside
// ``json.value.boolean``.
JSON _JSONInitBool(const json_bool_t boolean) {
  JSON json = _JSONInitType(JSON_Boolean);
  json.value.boolean = boolean;
  return json;
}

// Creates a ``JSON`` instance from a ``json_list_t*`` pointer type.
//
// Saves a ``json_list_t*`` value by going over the ``data`` attribute of
// ``Vector`` instance and copying the addresses of the values the ``data``
// linked list is pointing to.
//
// Note: This function does not copy the ``void*`` instances in the free-store
// instead only creates a new table in the free-store while keeping the pointer
// to the initial values regardless of their scope in the memory.  So if the
// initial data dies because it was allocated in stack not in the free-store
// then the ``JSON`` instance will lose that data too, so make sure to pass in
// dynamically allocated data.
JSON _JSONInitList(json_list_t* list) {
  JSON json = _JSONInitType(JSON_List);
  VectorCopy(&json.value.list, list);
  return json;
}

// Creates a ``JSON`` instance from a ``json_object_t*`` pointer type.
//
// Saves a ``json_object_t*`` value by going over the ``buckets`` attribute of
// the ``Map`` instance and copying the addresses of the values the ``buckets``
// linked list is pointing to.
//
// Note: This function does not copy the ``MapEntry`` instances in the
// free-store instead only creates a new table in the free-store while keeping
// the pointer to the initial values regardless of their scope in the memory.
// So if the initial data dies because it was allocated in stack not in the
// free-store then the ``JSON`` instance will lose that data too, so make sure
// to pass in dynamically allocated data.
JSON _JSONInitObject(json_object_t* map) {
  JSON json = _JSONInitType(JSON_Object);
  MapCopy(&json.value.map, map);
  return json;
}

JSON* JSONAllocType(JSON_type type) {
  return JSONAllocTypeSize(type, sizeof(JSON));
}

JSON* JSONAllocTypeSize(JSON_type type, size_t size) {
  JSON* json = (JSON*)(malloc(size));
  switch (type) {
    case JSON_Null:
      json->value.null = NULL;
      break;
    case JSON_String:
      json->value.string = (char*)(malloc(size * sizeof(char)));
      break;
    case JSON_Decimal:
      json->value.decimal = 0.0;
      break;
    case JSON_Number:
      json->value.number = 0;
      break;
    case JSON_Boolean:
      json->value.boolean = FALSE;
      break;
    case JSON_List:
      json->value.list = VectorAlloc(size);
      break;
    case JSON_Object:
      json->value.map = MapAllocNStrAsKey(size);
      break;
  }
  return json;
}

void JSONFree(JSON* const json) {
  if (json->type == JSON_String) {
    free(json->value.string);
  } else if (json->type == JSON_List) {
    void* current = NULL;
    VectorIterator vector_it = VectorIteratorNew(&json->value.list);
    while ((current = VectorIteratorNext(&vector_it)))
      JSONFree((JSON*)current);
    VectorFree(&json->value.list);
  } else if (json->type == JSON_Object) {
    MapEntry* current = NULL;
    MapIterator map_it = MapIteratorNew(&json->value.map);
    while ((current = MapIteratorNext(&map_it)))
      JSONFree((JSON*)current);
    MapFree(&json->value.map);
  } else if (json->type == JSON_Number) {
    json->value.number = 0;
  } else if (json->type == JSON_Decimal) {
    json->value.decimal = 0.0;
  } else if (json->type = JSON_Boolean) {
    json->value.boolean = FALSE;
  } else {
    json->value.null = NULL;
  }
}

void JSONFreeDeep(JSON* const json) {
  if (json->type == JSON_String) {
    free(json->value.string);
  } else if (json->type == JSON_List) {
    void* current = NULL;
    VectorIterator vector_it = VectorIteratorNew(&json->value.list);
    while ((current = VectorIteratorNext(&vector_it)))
      JSONFreeDeep((JSON*)current);
    VectorFreeDeep(&json->value.list);
  } else if (json->type == JSON_Object) {
    MapEntry* current = NULL;
    MapIterator map_it = MapIteratorNew(&json->value.map);
    while ((current = MapIteratorNext(&map_it)))
      JSONFreeDeep((JSON*)current);
    MapFreeDeep(&json->value.map);
  } else if (json->type == JSON_Number) {
    json->value.number = 0;
  } else if (json->type == JSON_Decimal) {
    json->value.decimal = 0.0;
  } else if (json->type = JSON_Boolean) {
    json->value.boolean = FALSE;
  } else {
    json->value.null = NULL;
  }
}
