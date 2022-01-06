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

#ifndef CJSON_TESTS_CJSON_TESTACCESSORS_HH_
#define CJSON_TESTS_CJSON_TESTACCESSORS_HH_

#include <cstring>

#include "bool.h"
#include "cjson.h"
#include "data/sstream/sstream.h"

class CJSONTest : public testing::Test {
 protected:
  void TearDown() override { JSONFree(&json); }

 protected:
  JSON json;
};

TEST_F(CJSONTest, JSONStringifiyTestWithAListAndAObjectInstance) {
  JSON json = JSONType(JSON_object);

  JSON list = JSONType(JSON_list);
  JSONListAddString(
      &list,
      (const json_string_t) "Mohika says \"Fake people leave when you cry.\"");
  JSONListAddDecimal(&list, 1.29);
  JSONListAddNull(&list);

  JSON object = JSONType(JSON_object);
  JSONObjectPutNull(&object, (const json_string_t) "Hope in my life");
  JSONObjectPutDecimal(&object, (const json_string_t) "My chances of success",
                       0.000000000000000);
  JSONObjectPutString(&object, (const json_string_t) "My horoscope",
                      (const json_string_t) "Just die");

  JSONObjectPut(&json, (const json_string_t) "list", &list);
  JSONObjectPut(&json, (const json_string_t) "object", &object);

  const char* json_expected_output =
      "{\n"
      "    \"list\": [\n"
      "        \"Mohika says \"Fake people leave when you cry.\",\n"
      "        1.299999,\n"
      "        null,\n"
      "        ]\n"
      "    \"object\": {\n"
      "        \"Hope in my life\": null,\n"
      "        \"My chances of success\": 0.000000000000000,\n"
      "        \"My horoscope\": \"Just die\"\n"
      "        }\n"
      "}\n";

  StringStream json_actual_data = JSONStringify(&json, TRUE, 4, TRUE);

  ASSERT_EQ(std::strcmp(json_actual_data.data, json_expected_output), 0)
      << "Recieved stringified data:\n"
      << json_actual_data.data << "Expeceted stringified data:\n"
      << json_expected_output;
}

#endif  // CJSON_TESTS_TESTACCESSORS_HH_
