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

#ifndef CJSON_TESTS_MAP_TESTITERATORS_HH_
#define CJSON_TESTS_MAP_TESTITERATORS_HH_

#include <gtest/gtest.h>

#include <cstring>

#include "bool.h"
#include "map/map.h"

class MapTraverseTest : public ::testing::Test {
  void SetUp() override { MapInit(&map, 10, Hash, KeyCmp); }

  void TearDown() override { MapFree(&map); }

 protected:
  Map map;
};

static int kCount1 = 0;
bool_t Predicate1(const void* key, const void* value) {
  ++(kCount1);
  return TRUE;
}

TEST_F(MapTraverseTest, TraversesEntireMap) {
  MapInsert(&map, "key1", std::strlen("key1"), "value1", std::strlen("value1"));
  MapInsert(&map, "key2", std::strlen("key2"), "value2", std::strlen("value2"));
  MapInsert(&map, "key3", std::strlen("key3"), "value3", std::strlen("value3"));
  MapInsert(&map, "key4", std::strlen("key4"), "value4", std::strlen("value4"));
  MapInsert(&map, "key5", std::strlen("key5"), "value5", std::strlen("value5"));
  MapInsert(&map, "key6", std::strlen("key6"), "value6", std::strlen("value6"));

  MapTraverse(&map, Predicate1);

  EXPECT_EQ(kCount1, 6);
}

static int kCount2 = 0;
bool_t Predicate2(const void* key, const void* value) {
  ++(kCount2);
  return std::strcmp(reinterpret_cast<const char*>(key), "key1") != 0 ? TRUE
                                                                      : FALSE;
}

TEST_F(MapTraverseTest, StopsTraversalOnFalsePredicate) {
  MapInsert(&map, "key1", std::strlen("key1"), "value1", std::strlen("value1"));
  MapInsert(&map, "key2", std::strlen("key2"), "value2", std::strlen("value2"));
  MapInsert(&map, "key3", std::strlen("key3"), "value3", std::strlen("value3"));

  MapTraverse(&map, Predicate2);

  EXPECT_EQ(kCount2, 2);
}

#endif  // CJSON_TESTS_MAP_TESTITERATORS_HH_
