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

#include "modifiers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson.h"
#include "data/map/map.h"
#include "data/vector/vector.h"

void JSONListAdd(JSON* const list, JSON* const value) {
  VectorPush(&list->value.list, (void*)value);
}

#define __json_copy_and_insert_into_json_list(json, obj_to_copy, list) \
  do {                                                                 \
    memcpy(json, &obj_to_copy, sizeof(JSON));                          \
    JSONListAdd(list, json);                                           \
  } while (0)

#define __json_add_null_value_to_json_list(type, list)       \
  do {                                                       \
    JSON* json = (JSON*)(malloc(sizeof(JSON)));              \
    JSON type = JSON_INIT(type);                             \
    __json_copy_and_insert_into_json_list(json, type, list); \
  } while (0)

#define __json_add_value_to_json_list(type, list, value)     \
  do {                                                       \
    JSON* json = (JSON*)(malloc(sizeof(JSON)));              \
    JSON type = JSON_INIT_VAL(type, value);                  \
    __json_copy_and_insert_into_json_list(json, type, list); \
  } while (0)

void _JSONListAddNull(JSON* const list) {
  __json_add_null_value_to_json_list(Null, list);
}

void _JSONListAddNumber(JSON* const list, const json_number_t value) {
  __json_add_value_to_json_list(Number, list, value);
}

void _JSONListAddDecimal(JSON* const list, const json_decimal_t value) {
  __json_add_value_to_json_list(Decimal, list, value);
}

void _JSONListAddBool(JSON* const list, const json_bool_t value) {
  __json_add_value_to_json_list(Bool, list, value);
}

void _JSONListAddString(JSON* const list, const json_string_t data) {
  __json_add_value_to_json_list(String, list, data);
}

void JSONObjectPut(JSON* const object, const json_string_t const key,
                   JSON* const value) {
  MapPut(&object->value.map, (void*)key, (void*)value);
}

#define __json_copy_and_insert_into_json_object(json, obj_to_copy, key, \
                                                object)                 \
  do {                                                                  \
    memcpy(json, &obj_to_copy, sizeof(JSON));                           \
    JSONObjectPut(object, key, json);                                   \
  } while (0)

#define __json_add_null_value_to_json_object(type, key, object)       \
  do {                                                                \
    JSON* json = (JSON*)(malloc(sizeof(JSON)));                       \
    JSON type = JSON_INIT(type);                                      \
    __json_copy_and_insert_into_json_object(json, type, key, object); \
  } while (0)

#define __json_add_value_to_json_object(type, object, key, value)     \
  do {                                                                \
    JSON* json = (JSON*)(malloc(sizeof(JSON)));                       \
    JSON type = JSON_INIT_VAL(type, value);                           \
    __json_copy_and_insert_into_json_object(json, type, key, object); \
  } while (0)

void _JSONObjectPutNull(JSON* const object, const json_string_t const key) {
  __json_add_null_value_to_json_object(Null, key, object);
}

void _JSONObjectPutNumber(JSON* const object, const json_string_t const key,
                          const json_number_t value) {
  __json_add_value_to_json_object(Number, object, key, value);
}

void _JSONObjectPutDecimal(JSON* const object, const json_string_t const key,
                           const json_decimal_t value) {
  __json_add_value_to_json_object(Decimal, object, key, value);
}

void _JSONObjectPutBool(JSON* const object, const json_string_t const key,
                        const json_bool_t value) {
  __json_add_value_to_json_object(Bool, object, key, value);
}

void _JSONObjectPutString(JSON* const object, const json_string_t const key,
                          json_string_t const data) {
  __json_add_value_to_json_object(String, object, key, data);
}
