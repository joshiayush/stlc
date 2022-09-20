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

#include "accessors.h"

#include <stdio.h>

#include "bool.h"
#include "bytes.h"
#include "cjson.h"
#include "data/sstream/sstream.h"
#include "data/vector/vector.h"

#define JSON_TAB "    "

StringStream JSON_Stringify(JSON* const json, const bool_t prettify,
                            const size_t init_tab_pos,
                            const bool_t is_dict_valid) {
  StringStream stringified;

#define __place_endl_if_prettify ((prettify) ? (endl) : (""))

  if (prettify && !is_dict_valid) {
    stringified = StringStreamNAlloc(4 * init_tab_pos);
    for (size_t i = 0; i < init_tab_pos; ++i)
      StringStreamConcat(&stringified, JSON_TAB);
  } else {
    stringified = StringStreamAlloc();
  }

  switch (json->type) {
    case JSON_Null:
      StringStreamConcat(&stringified, JSON_NULL);
      break;
    case JSON_String:
      StringStreamConcat(&stringified, "\"%s\"", json->value.string);
      break;
    case JSON_Number:
      StringStreamConcat(&stringified, "%lld", json->value.number);
      break;
    case JSON_Decimal:
      StringStreamConcat(&stringified, "%lf", json->value.decimal);
      break;
    case JSON_Boolean:
      StringStreamConcat(&stringified, "%s",
                         json->value.boolean ? JSON_TRUE : JSON_FALSE);
      break;
    case JSON_List: {
      if (!json->value.list.size) {
        StringStreamConcat(&stringified, "[]%s", __place_endl_if_prettify);
        break;
      }

      StringStreamConcat(&stringified, "[%s", __place_endl_if_prettify);
      void* current = NULL;
      VectorIterator vectorit = VectorIteratorNew(&json->value.list);
      while ((current = VectorIteratorNext(&vectorit))) {
        StringStream sstream =
            JSON_Stringify((JSON*)current, prettify, init_tab_pos + 1, FALSE);
        StringStreamConcat(&stringified, "%s,%s", sstream.data,
                           __place_endl_if_prettify);
        StringStreamDealloc(&sstream);
      }

      StringStreamRetreat(&stringified, prettify ? 2 : 1);

      if (prettify) {
        StringStreamConcat(&stringified, endl);
        for (size_t i = 0; i < init_tab_pos; ++i)
          StringStreamConcat(&stringified, JSON_TAB);
      }

      StringStreamConcat(&stringified, "]%s", __place_endl_if_prettify);
      break;
    }
    case JSON_Object: {
      if (!json->value.object.entrieslen) {
        StringStreamConcat(&stringified, "{}%s", __place_endl_if_prettify);
        break;
      }

      StringStreamConcat(&stringified, "{%s", __place_endl_if_prettify);
      MapEntry* current = NULL;
      MapIterator objectit = MapIteratorNew(&json->value.object);
      while ((current = MapIteratorNext(&objectit))) {
        if (prettify) {
          for (size_t i = 0; i < init_tab_pos; ++i)
            StringStreamConcat(&stringified, JSON_TAB);
        }
        StringStreamConcat(&stringified, "\"%s\":%s", (char*)current->key,
                           prettify ? " " : "");
        StringStream sstream = JSON_Stringify((JSON*)current->value, prettify,
                                              init_tab_pos + 1, TRUE);
        StringStreamConcat(&stringified, "%s,%s", sstream.data,
                           __place_endl_if_prettify);
      }

      StringStreamRetreat(&stringified, prettify ? 2 : 1);

      if (prettify) {
        StringStreamConcat(&stringified, __place_endl_if_prettify);
        for (size_t i = 0; i < init_tab_pos; ++i)
          StringStreamConcat(&stringified, JSON_TAB);
      }

      StringStreamConcat(&stringified, "}");
      break;
    }
  }

  return stringified;
}
