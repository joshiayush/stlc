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

#ifndef INCLUDE_CJSON_H_
#define INCLUDE_CJSON_H_

#include <sys/types.h>

#include "bool.h"
#include "data/map/map.h"
#include "data/sstream/sstream.h"
#include "data/vector/vector.h"

typedef void* json_null_t;
typedef char* json_string_t;
typedef __int64_t json_number_t;
typedef double json_decimal_t;
typedef bool_t json_bool_t;
typedef Vector json_list_t;
typedef Map json_object_t;

// ``JSON_type`` stores the different ``JSON`` types that we can possibly have.
// This ``enum`` is used to identify the type of the ``JSON_value`` instance
// currently holding in it.
enum JSON_type {
  JSON_null = 0,
  JSON_string = 1,
  JSON_number = 2,
  JSON_decimal = 3,
  JSON_boolean = 4,
  JSON_list = 5,
  JSON_object = 6
};

// ``JSON_value`` union stores ``JSON`` style values.  ``union`` is preferred
// over a ``struct`` to save memory.  Only one the value at a time can be stored
// while the others will be currupted.
typedef union JSON_value {
  json_null_t null;
  json_string_t string;
  json_number_t number;
  json_decimal_t decimal;
  json_bool_t boolean;
  json_list_t list;
  json_object_t map;
} JSON_value;

typedef struct JSON {
  JSON_type type;
  JSON_value value;
} JSON;

JSON JSONType(JSON_type type);
JSON JSONTypeSize(JSON_type type, size_t size);

JSON JSONNull();
JSON JSONString(const json_string_t string);
JSON JSONNumber(const json_number_t number);
JSON JSONDecimal(const json_decimal_t decimal);
JSON JSONBool(const json_bool_t boolean);
JSON JSONList(const json_list_t* list);
JSON JSONObject(const json_object_t* map);

JSON* JSONAllocType(JSON_type type);
JSON* JSONAllocTypeSize(JSON_type type, size_t size);

#endif  // INCLUDE_CJSON_H_
