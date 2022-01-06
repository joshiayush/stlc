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

void JSONListAddNull(JSON* const list) {
  JSON* json = (JSON*)malloc(sizeof(JSON));
  JSON json_null = JSONNull();
  memcpy(json, &json_null, sizeof(JSON));
  JSONListAdd(list, json);
}

void JSONListAddNumber(JSON* const list, const json_number_t value) {
  JSON* json = (JSON*)malloc(sizeof(JSON));
  JSON json_number = JSONNumber(value);
  memcpy(json, &json_number, sizeof(JSON));
  JSONListAdd(list, json);
}

void JSONListAddDecimal(JSON* const list, const json_decimal_t value) {
  JSON* json = (JSON*)malloc(sizeof(JSON));
  JSON json_decimal = JSONDecimal(value);
  memcpy(json, &json_decimal, sizeof(JSON));
  JSONListAdd(list, json);
}

void JSONListAddBool(JSON* const list, const json_bool_t value) {
  JSON* json = (JSON*)malloc(sizeof(JSON));
  JSON json_boolean = JSONBool(value);
  memcpy(json, &json_boolean, sizeof(JSON));
  JSONListAdd(list, json);
}

void JSONListAddString(JSON* const list, const json_string_t data) {
  JSON* json = (JSON*)malloc(sizeof(JSON));
  JSON json_string = JSONString(data);
  memcpy(json, &json_string, sizeof(JSON));
  JSONListAdd(list, json);
}

void JSONObjectPut(JSON* const object, const json_string_t const key,
                   JSON* const value) {
  MapPut(&object->value.map, (void*)key, (void*)value);
}

void JSONObjectPutNull(JSON* const object, const json_string_t const key) {
  JSON* json = (JSON*)malloc(sizeof(JSON));
  JSON json_null = JSONNull();
  memcpy(json, &json_null, sizeof(JSON));
  JSONObjectPut(object, key, json);
}

void JSONObjectPutNumber(JSON* const object, const json_string_t const key,
                         const json_number_t value) {
  JSON* json = (JSON*)malloc(sizeof(JSON));
  JSON json_number = JSONNumber(value);
  memcpy(json, &json_number, sizeof(JSON));
  JSONObjectPut(object, key, json);
}

void JSONObjectPutDecimal(JSON* const object, const json_string_t const key,
                          const json_decimal_t value) {
  JSON* json = (JSON*)malloc(sizeof(JSON));
  JSON json_decimal = JSONDecimal(value);
  memcpy(json, &json_decimal, sizeof(JSON));
  JSONObjectPut(object, key, json);
}

void JSONObjectPutBool(JSON* const object, const json_string_t const key,
                       const json_bool_t value) {
  JSON* json = (JSON*)malloc(sizeof(JSON));
  JSON json_boolean = JSONBool(value);
  memcpy(json, &json_boolean, sizeof(JSON));
  JSONObjectPut(object, key, json);
}

void JSONObjectPutString(JSON* const object, const json_string_t const key,
                         json_string_t const data) {
  JSON* json = (JSON*)malloc(sizeof(JSON));
  JSON json_string = JSONString(data);
  memcpy(json, &json_string, sizeof(JSON));
  JSONObjectPut(object, key, json);
}
