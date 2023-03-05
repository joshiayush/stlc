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

#include <assert.h>
#include <gtest/gtest.h>

#include <cstring>

#include "bool.h"
#include "map/map.h"
#include "map/ops.h"
#include "utils.hh"

TEST(HashTest, SameStringReturnsSameHashValue) {
  const char* key = "hello world";
  hash_t hash1 = Hash(key);
  hash_t hash2 = Hash(key);
  EXPECT_EQ(hash1, hash2);
}

TEST(HashTest, DifferentStringsReturnDifferentHashValues) {
  const char* key1 = "hello";
  const char* key2 = "world";
  hash_t hash1 = Hash(key1);
  hash_t hash2 = Hash(key2);
  EXPECT_NE(hash1, hash2);
}

TEST(HashTest, SameStringWithDifferentCasesReturnsDifferentHashValue) {
  const char* key1 = "hello world";
  const char* key2 = "Hello World";
  hash_t hash1 = Hash(key1);
  hash_t hash2 = Hash(key2);
  EXPECT_NE(hash1, hash2);
}

TEST(HashTest, NullInputReturnsSameHashValue) {
  hash_t hash1 = Hash(NULL);
  hash_t hash2 = Hash(NULL);
  EXPECT_EQ(hash1, hash2);
}

TEST(KeyCmpTest, NullInputs) {
  EXPECT_EQ(KeyCmp(NULL, NULL), TRUE);

  EXPECT_EQ(KeyCmp(NULL, "test"), FALSE);
  EXPECT_EQ(KeyCmp("test", NULL), FALSE);
}

TEST(KeyCmpTest, DifferentInputs) {
  EXPECT_EQ(KeyCmp("test1", "test2"), FALSE);
  EXPECT_EQ(KeyCmp("test", "hello"), FALSE);
}

TEST(KeyCmpTest, SameInputs) {
  EXPECT_EQ(KeyCmp("test", "test"), TRUE);
  EXPECT_EQ(KeyCmp("hello world", "hello world"), TRUE);
}

class MapEntryInitTest : public ::testing::Test {
 protected:
  void SetUp() override {
    key = "name";
    value = "Ritu";
    key_size = strlen(key) + 1;
    value_size = strlen(value) + 1;
    hash = Hash(key);
  }

  void TearDown() override {}

 protected:
  const char* key;
  const char* value;
  size_t key_size;
  size_t value_size;
  hash_t hash;
};

TEST_F(MapEntryInitTest, SavesTheGivenValues) {
  MapEntry entry;
  MapEntryInit(&entry, key, key_size, value, value_size, hash);

  ASSERT_STREQ((char*)entry.key, key);
  ASSERT_STREQ((char*)entry.value, value);
  ASSERT_EQ(entry.hash, hash);
  ASSERT_EQ(entry.next, nullptr);

  free(entry.key);
  free(entry.value);
}

TEST_F(MapEntryInitTest, NullArgsTest) {
  MapEntry entry;
  MapEntryInit(&entry, nullptr, key_size, nullptr, value_size, hash);

  ASSERT_EQ(entry.key, nullptr);
  ASSERT_EQ(entry.value, nullptr);
  ASSERT_EQ(entry.hash, hash);
  ASSERT_EQ(entry.next, nullptr);
}

TEST_F(MapEntryInitTest, EmptyArgsTest) {
  MapEntry entry;
  MapEntryInit(&entry, "", 1, "", 1, hash);

  ASSERT_STREQ((char*)entry.key, "");
  ASSERT_STREQ((char*)entry.value, "");
  ASSERT_EQ(entry.hash, hash);
  ASSERT_EQ(entry.next, nullptr);

  free(entry.key);
  free(entry.value);
}

TEST(MapInitTest, InitWithExplicitValues) {
  Map map;
  MapInit(&map, 10, Hash, KeyCmp);

  EXPECT_EQ(map.capacity, 10);
  EXPECT_EQ(map.size, 0);
  EXPECT_EQ(map.hash_func, Hash);
  EXPECT_EQ(map.key_eq_func, KeyCmp);
  EXPECT_NE(map.buckets, nullptr);

  MapFree(&map);
}

TEST(MapInitTest, InitANullMap) {
  Map* map = nullptr;
  MapInit(map, 10, Hash, KeyCmp);
  EXPECT_EQ(map, nullptr);
}

TEST(MapInitTest, InitNullHashFunc) {
  Map map;
  MapInit(&map, 10, nullptr, KeyCmp);
  EXPECT_EQ(map.hash_func, nullptr);
  MapFree(&map);
}

TEST(MapInitTest, InitNullKeyEqFunc) {
  Map map;
  MapInit(&map, 10, Hash, nullptr);
  EXPECT_EQ(map.key_eq_func, nullptr);
  MapFree(&map);
}

#endif  // CJSON_TESTS_MAP_TESTMAP_HH_
