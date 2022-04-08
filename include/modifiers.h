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

#ifndef CJSON_INCLUDE_MODIFIERS_H_
#define CJSON_INCLUDE_MODIFIERS_H_

#include "cjson.h"

#ifdef __cplusplus
extern "C" {
#endif

void JSONListAdd(JSON* const list, JSON* const value);
void _JSONListAddNull(JSON* const list);
void _JSONListAddNumber(JSON* const list, const json_number_t value);
void _JSONListAddDecimal(JSON* const list, const json_decimal_t value);
void _JSONListAddBool(JSON* const list, const json_bool_t value);
void _JSONListAddString(JSON* const list, const json_string_t data);

#define JSON_LIST_ADD(value_type, json_inst) _JSONListAdd##value_type(json_inst)
#define JSON_LIST_ADD_VAL(value_type, json_inst, value) \
  _JSONListAdd##value_type(json_inst, value)

void JSONObjectPut(JSON* const object, const json_string_t key,
                   JSON* const value);
void _JSONObjectPutNull(JSON* const object, const json_string_t key);
void _JSONObjectPutNumber(JSON* const object, const json_string_t key,
                          const json_number_t value);
void _JSONObjectPutDecimal(JSON* const object, const json_string_t key,
                           const json_decimal_t value);
void _JSONObjectPutBool(JSON* const object, const json_string_t key,
                        const json_bool_t value);
void _JSONObjectPutString(JSON* const object, const json_string_t key,
                          const json_string_t data);

#define JSON_OBJECT_PUT(value_type, json_inst, key) \
  _JSONObjectPut##value_type(json_inst, key)
#define JSON_OBJECT_PUT_VAL(value_type, json_inst, key, value) \
  _JSONObjectPut##value_type(json_inst, key, value)

#ifdef __cplusplus
}
#endif

#endif  // CJSON_MODIFIERS_H_
