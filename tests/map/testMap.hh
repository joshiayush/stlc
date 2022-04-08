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

#ifndef CJSON_TESTS_MAP_TESTMAP_HH_
#define CJSON_TESTS_MAP_TESTMAP_HH_

#include <gtest/gtest.h>

#include <cstring>

#include "data/map/map.h"
#include "data/map/ops.h"

class MapTest : public testing::Test {
 protected:
  void TearDown() override { MapFree(&map); }

 protected:
  Map map;
};

TEST_F(MapTest, MapInstanceTestWhenAllocatedWithMapAllocStrAsKey) {
  map = MapAllocStrAsKey();

  char mohika[7];
  std::strcpy(mohika, "Mohika");
  char mohika_description[20];
  std::strcpy(mohika_description, "Very sweet girl :).");
  char ayush[6];
  std::strcpy(ayush, "Ayush");
  char ayush_description[26];
  std::strcpy(ayush_description, "Still searching who I am.");

  MapPut(&map, mohika, mohika_description);
  MapPut(&map, ayush, ayush_description);

  ASSERT_NE(MapGet(&map, mohika), (void*)0);
  ASSERT_EQ(std::strcmp((char*)MapGet(&map, mohika), mohika_description), 0);
  ASSERT_NE(MapGet(&map, ayush), (void*)0);
  ASSERT_EQ(std::strcmp((char*)MapGet(&map, ayush), ayush_description), 0);
}

TEST_F(MapTest, TestMapRemoveFunctionWhenAllocatedWithMapAllocStrAsKey) {
  map = MapAllocStrAsKey();

  char mohika[7];
  std::strcpy(mohika, "Mohika");
  char mohika_description[20];
  std::strcpy(mohika_description, "Very sweet girl :).");
  char ayush[6];
  std::strcpy(ayush, "Ayush");
  char ayush_description[26];
  std::strcpy(ayush_description, "Still searching who I am.");

  MapPut(&map, mohika, mohika_description);
  MapPut(&map, ayush, ayush_description);

  ASSERT_NE(MapGet(&map, mohika), (void*)0);
  ASSERT_EQ(std::strcmp((char*)MapGet(&map, mohika), mohika_description), 0);
  ASSERT_NE(MapGet(&map, ayush), (void*)0);
  ASSERT_EQ(std::strcmp((char*)MapGet(&map, ayush), ayush_description), 0);

  const void* value = MapRemove(&map, ayush);
  ASSERT_NE(value, (void*)0);
  ASSERT_EQ(std::strcmp((char*)value, ayush_description), 0);
  ASSERT_EQ(MapGet(&map, ayush), (void*)0);
}

#endif  // CJSON_TESTS_MAP_TESTMAP_HH_
