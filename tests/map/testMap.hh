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
    key_size = std::strlen(key) + 1;
    value_size = std::strlen(value) + 1;
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
  MapEntryInit(&entry, key, key_size, value, value_size, hash, nullptr);

  ASSERT_STREQ((char*)entry.key, key);
  ASSERT_STREQ((char*)entry.value, value);
  ASSERT_EQ(entry.hash, hash);
  ASSERT_EQ(entry.next, nullptr);

  free(entry.key);
  free(entry.value);
}

TEST_F(MapEntryInitTest, NullArgsTest) {
  MapEntry entry;
  MapEntryInit(&entry, nullptr, key_size, nullptr, value_size, hash, nullptr);

  ASSERT_EQ(entry.key, nullptr);
  ASSERT_EQ(entry.value, nullptr);
  ASSERT_EQ(entry.hash, hash);
  ASSERT_EQ(entry.next, nullptr);
}

TEST_F(MapEntryInitTest, EmptyArgsTest) {
  MapEntry entry;
  MapEntryInit(&entry, "", 1, "", 1, hash, nullptr);

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

void* MapThreadFunc(void* arg) {
  Map* map = (Map*)arg;

  for (int i = 0; i < 1000; i++) {
    MapInsert(map, "key", std::strlen("key") + 1, "value",
              std::strlen("value") + 1);
    MapGet(map, "key");
    MapRemove(map, "key", std::strlen("key") + 1);
  }

  return NULL;
}

TEST(MapMutexTest, ThreadSafety) {
  Map map;
  MapInit(&map, 10, Hash, KeyCmp);

  const char* keys[] = {"key1", "key2", "key3", "key4", "key5"};
  const char* values[] = {"value1", "value2", "value3", "value4", "value5"};
  const int num_entries = sizeof(keys) / sizeof(keys[0]);
  for (int i = 0; i < num_entries; i++) {
    MapInsert(&map, keys[i], std::strlen(keys[i]) + 1, values[i],
              std::strlen(values[i]) + 1);
  }

  const int num_threads = 10;
  pthread_t threads[num_threads];
  for (int i = 0; i < num_threads; i++) {
    pthread_create(&threads[i], NULL, MapThreadFunc, &map);
  }

  for (int i = 0; i < num_threads; i++) pthread_join(threads[i], NULL);

  for (int i = 0; i < num_entries; i++) {
    MapEntry entry;
    MapEntryInit(&entry, keys[i], std::strlen(keys[i]), MapGet(&map, keys[i]),
                 std::strlen((char*)MapGet(&map, keys[i])),
                 map.hash_func(keys[i]), nullptr);
    ASSERT_STREQ((char*)entry.value, values[i]);
  }

  MapFree(&map);
}

TEST(MapReallocTest, EmptyMapRealloc) {
  Map map;
  MapInit(&map, 2, Hash, KeyCmp);
  MapRealloc(&map, 4);

  EXPECT_EQ(map.capacity, 4);
  EXPECT_EQ(map.size, 0);

  MapFree(&map);
}

TEST(MapReallocTest, NonEmptyMapRealloc) {
  Map map;
  MapInit(&map, 4, Hash, KeyCmp);

  MapInsert(&map, "key1", std::strlen("key1"), "value1", std::strlen("value1"));
  MapInsert(&map, "key2", std::strlen("key2"), "value2", std::strlen("value2"));
  MapInsert(&map, "key3", std::strlen("key3"), "value3", std::strlen("value3"));
  MapInsert(&map, "key4", std::strlen("key4"), "value4", std::strlen("value4"));

  MapRealloc(&map, 8);

  EXPECT_EQ(map.capacity, 8);
  EXPECT_EQ(map.size, 4);

  MapFree(&map);
}

TEST(MapReallocTest, MapReallocWithSameCapacity) {
  Map map;
  MapInit(&map, 4, Hash, KeyCmp);

  MapInsert(&map, "key1", std::strlen("key1"), "value1", std::strlen("value1"));
  MapInsert(&map, "key2", std::strlen("key2"), "value2", std::strlen("value2"));
  MapInsert(&map, "key3", std::strlen("key3"), "value3", std::strlen("value3"));
  MapInsert(&map, "key4", std::strlen("key4"), "value4", std::strlen("value4"));

  MapRealloc(&map, 4);

  EXPECT_EQ(map.capacity, 4);
  EXPECT_EQ(map.size, 4);

  MapFree(&map);
}

TEST(MapReallocTest, MapReallocWithSmallerCapacity) {
  Map map;
  MapInit(&map, 8, Hash, KeyCmp);

  MapInsert(&map, "key1", std::strlen("key1"), "value1", std::strlen("value1"));
  MapInsert(&map, "key2", std::strlen("key2"), "value2", std::strlen("value2"));
  MapInsert(&map, "key3", std::strlen("key3"), "value3", std::strlen("value3"));
  MapInsert(&map, "key4", std::strlen("key4"), "value4", std::strlen("value4"));
  MapInsert(&map, "key5", std::strlen("key5"), "value5", std::strlen("value5"));
  MapInsert(&map, "key6", std::strlen("key6"), "value6", std::strlen("value6"));
  MapInsert(&map, "key7", std::strlen("key7"), "value7", std::strlen("value7"));
  MapInsert(&map, "key8", std::strlen("key8"), "value8", std::strlen("value8"));

  MapRealloc(&map, 4);

  EXPECT_EQ(map.capacity, 4);
  EXPECT_EQ(map.size, 4);

  MapFree(&map);
}

#endif  // CJSON_TESTS_MAP_TESTMAP_HH_
