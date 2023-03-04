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

static hash_t CustomHash(const void* const key) {
  hash_t hash = 0;
  if (key == nullptr)
    return hash;
  for (const char* c = reinterpret_cast<const char*>(key); *c != '\0'; ++c)
    hash += static_cast<hash_t>(*c);
  return hash;
}

static bool_t CustomKeyCmp(const void* key1, const void* key2) { return TRUE; }

TEST(MapMacrosTest,
     TestIfAnyExternalFlagWhileBuildingLibraryDidChangeTheValues) {
  EXPECT_EQ(MAX_LOAD_FACTOR, 0.75f);
  EXPECT_EQ(MAP_DEFAULT_BUCKET_LEN, (1 << 4));
}

TEST(MapEntryStructTest, TestSizeOfMapEntry) {
  EXPECT_EQ(sizeof(MapEntry), 32UL)
      << "Error: sizeof(MapEntry) = " << sizeof(MapEntry);
}

TEST(MapTestStructTest, TestSizeOfMap) {
  EXPECT_EQ(sizeof(Map), 40UL) << "Error: sizeof(Map) = " << sizeof(Map);
}

TEST(MapAllocEntryWithHashTest, TestWhenA16DigitsHashIsUsed) {
  MapEntry* mapentry =
      MapAllocEntryWithHash("key1", "value1", 0xbabe7cee878d3e62);
  ASSERT_NE(mapentry, nullptr);
  EXPECT_STREQ((const char*)mapentry->key, "key1");
  EXPECT_STREQ((const char*)mapentry->value, "value1");
  EXPECT_EQ(mapentry->hash, 0xbabe7cee878d3e62);
  EXPECT_EQ(mapentry->next, nullptr);
  delete mapentry;
}

TEST(MapAllocEntryWithHashTest, TestWhenMultipleEntriesAreLinkedTogether) {
  MapEntry* mapentry1 =
      MapAllocEntryWithHash("key1", "value1", 0xbabe7cee878d3e62);
  MapEntry* mapentry2 =
      MapAllocEntryWithHash("key2", "value2", 0x280f93cdf26cf053);
  MapEntry* mapentry3 =
      MapAllocEntryWithHash("key3", "value3", 0xd53239e006c95f4a);

  ASSERT_NE(mapentry1, nullptr);
  ASSERT_NE(mapentry2, nullptr);
  ASSERT_NE(mapentry3, nullptr);

  mapentry1->next = mapentry2;
  mapentry2->next = mapentry3;

  EXPECT_STREQ((const char*)mapentry1->key, "key1");
  EXPECT_STREQ((const char*)mapentry1->value, "value1");

  EXPECT_STREQ((const char*)mapentry2->key, "key2");
  EXPECT_STREQ((const char*)mapentry2->value, "value2");

  EXPECT_STREQ((const char*)mapentry3->key, "key3");
  EXPECT_STREQ((const char*)mapentry3->value, "value3");

  EXPECT_EQ(mapentry1->hash, 0xbabe7cee878d3e62);
  EXPECT_EQ(mapentry2->hash, 0x280f93cdf26cf053);
  EXPECT_EQ(mapentry3->hash, 0xd53239e006c95f4a);

  EXPECT_EQ(mapentry1->next, mapentry2);
  EXPECT_EQ(mapentry2->next, mapentry3);
  EXPECT_EQ(mapentry3->next, nullptr);

  delete mapentry1;
  delete mapentry2;
  delete mapentry3;
}

TEST(MapAllocTest, TestWhenNullIsGivenAsArguments) {
  Map map = MapAlloc(NULL, NULL);
  EXPECT_EQ(map.bucketslen, MAP_DEFAULT_BUCKET_LEN);
  EXPECT_EQ(map.entrieslen, 0);
  ASSERT_NE(map.buckets, nullptr);
  EXPECT_EQ(map.hash, Hash);
  EXPECT_EQ(map.keycmp, KeyCmp);
  for (size_t i = 0; i < map.bucketslen; ++i)
    EXPECT_EQ(map.buckets[i], nullptr)
        << "Bucket at index: " << i << " is not nullptr";
  MapFree(&map);
}

TEST(MapAllocTest, TestWhenDefaultFunctionsAreGivenAsArguments) {
  Map map = MapAlloc(Hash, KeyCmp);
  EXPECT_EQ(map.bucketslen, MAP_DEFAULT_BUCKET_LEN);
  EXPECT_EQ(map.entrieslen, 0);
  ASSERT_NE(map.buckets, nullptr);
  EXPECT_EQ(map.hash, Hash);
  EXPECT_EQ(map.keycmp, KeyCmp);
  for (size_t i = 0; i < map.bucketslen; ++i)
    EXPECT_EQ(map.buckets[i], nullptr)
        << "Bucket at index: " << i << " is not nullptr";
  MapFree(&map);
}

TEST(MapAllocTest, TestWhenCustomFunctionsAreGivenAsArguments) {
  Map map = MapAlloc(CustomHash, CustomKeyCmp);
  EXPECT_EQ(map.bucketslen, MAP_DEFAULT_BUCKET_LEN);
  EXPECT_EQ(map.entrieslen, 0);
  ASSERT_NE(map.buckets, nullptr);
  EXPECT_EQ(map.hash, CustomHash);
  EXPECT_EQ(map.keycmp, CustomKeyCmp);
  for (size_t i = 0; i < map.bucketslen; ++i)
    EXPECT_EQ(map.buckets[i], nullptr)
        << "Bucket at index: " << i << " is not nullptr";
  MapFree(&map);
}

TEST(MapAllocNBucketsTest, TestWhenBucketsLenIsZeroAndFunctionsAreNull) {
  Map map = MapAllocNBuckets(0, NULL, NULL);
  EXPECT_EQ(map.bucketslen, MAP_DEFAULT_BUCKET_LEN);
  EXPECT_EQ(map.entrieslen, 0);
  ASSERT_NE(map.buckets, nullptr);
  EXPECT_EQ(map.hash, Hash);
  EXPECT_EQ(map.keycmp, KeyCmp);
  for (size_t i = 0; i < map.bucketslen; ++i)
    EXPECT_EQ(map.buckets[i], nullptr)
        << "Bucket at index: " << i << " is not nullptr";
  MapFree(&map);
}

TEST(MapAllocNBucketsTest,
     TestWhenBucketsLenIsGivenAndFunctionsAreDefaultOnes) {
  Map map = MapAllocNBuckets(32UL, Hash, KeyCmp);
  EXPECT_EQ(map.bucketslen, 32UL);
  EXPECT_EQ(map.entrieslen, 0);
  ASSERT_NE(map.buckets, nullptr);
  EXPECT_EQ(map.hash, Hash);
  EXPECT_EQ(map.keycmp, KeyCmp);
  for (size_t i = 0; i < map.bucketslen; ++i)
    EXPECT_EQ(map.buckets[i], nullptr)
        << "Bucket at index: " << i << " is not nullptr";
  MapFree(&map);
}

TEST(MapAllocNBucketsTest, TestWhenBucketsLenIsGivenAndFunctionsAreCustomOnes) {
  Map map = MapAllocNBuckets(32UL, CustomHash, CustomKeyCmp);
  EXPECT_EQ(map.bucketslen, 32UL);
  EXPECT_EQ(map.entrieslen, 0);
  ASSERT_NE(map.buckets, nullptr);
  EXPECT_EQ(map.hash, CustomHash);
  EXPECT_EQ(map.keycmp, CustomKeyCmp);
  for (size_t i = 0; i < map.bucketslen; ++i)
    EXPECT_EQ(map.buckets[i], nullptr)
        << "Bucket at index: " << i << " is not nullptr";
  MapFree(&map);
}

TEST(MapAllocNEntriesTest, TestWhenEntriesLenGivenIsZero) {
  Map map = MapAllocNEntries(0, Hash, KeyCmp);
  EXPECT_EQ(map.bucketslen, MAP_DEFAULT_BUCKET_LEN);
  EXPECT_EQ(map.entrieslen, 0);
  ASSERT_NE(map.buckets, nullptr);
  EXPECT_EQ(map.hash, Hash);
  EXPECT_EQ(map.keycmp, KeyCmp);
  for (size_t i = 0; i < map.bucketslen; ++i)
    EXPECT_EQ(map.buckets[i], nullptr)
        << "Bucket at index: " << i << " is not nullptr";
  MapFree(&map);
}

TEST(MapAllocNEntriesTest,
     TestWhenEntriesLenGivenIsGreaterThanDefaultBucketsLen) {
  Map map = MapAllocNEntries(32UL, Hash, KeyCmp);
  EXPECT_EQ(map.bucketslen,
            cjson::testing::map::utils::ComputeBucketsLenSatisfyingLoadFactor(
                MAX_LOAD_FACTOR, 32UL));
  EXPECT_EQ(map.entrieslen, 0);
  ASSERT_NE(map.buckets, nullptr);
  EXPECT_EQ(map.hash, Hash);
  EXPECT_EQ(map.keycmp, KeyCmp);
  for (size_t i = 0; i < map.bucketslen; ++i)
    EXPECT_EQ(map.buckets[i], nullptr)
        << "Bucket at index: " << i << " is not nullptr";
  MapFree(&map);
}

static void MapPutWithoutReallocation(Map* map, const void* const key,
                                      const void* const value) {
  hash_t hash = map->hash(key);
  size_t idx = CalculateIndex(hash, map->bucketslen);

  MapEntry* mapentry = MapAllocEntryWithHash(key, value, hash);

  if (*(map->buckets + idx) == nullptr) {
    *(map->buckets + idx) = mapentry;
    ++map->entrieslen;
    return;
  } else {
    MapEntry* current = *(map->buckets + idx);
    while (current->next != nullptr)
      current = current->next;
    if ((hash == current->hash) && map->keycmp(key, current->key) == TRUE) {
      const size_t value_len = strlen((const char*)value);
      if (value_len > strlen((const char*)current->value)) {
        current->value = realloc(current->value, value_len * sizeof(char));
        assert(current->value != NULL);
      }
      strcpy((char*)current->value, (const char*)value);
    } else {
      MapEntry* tmp_mapentry = current->next;
      current->next = mapentry;
      mapentry->next = tmp_mapentry;
      ++(map->entrieslen);
    }
  }
}

TEST(MapReallocTest, TestWhenReallocationIsNotRequired) {
  Map map = MapAlloc(Hash, KeyCmp);

  EXPECT_EQ(map.entrieslen, 0);
  EXPECT_EQ(map.bucketslen, MAP_DEFAULT_BUCKET_LEN);
  EXPECT_EQ(map.hash, Hash);
  EXPECT_EQ(map.keycmp, KeyCmp);

  MapPutWithoutReallocation(&map, "key1", "value1");
  MapPutWithoutReallocation(&map, "key2", "value2");
  MapPutWithoutReallocation(&map, "key3", "value3");
  MapPutWithoutReallocation(&map, "key4", "value4");
  MapPutWithoutReallocation(&map, "key5", "value5");

  ASSERT_EQ(map.entrieslen, 5);
  ASSERT_EQ(map.bucketslen, MAP_DEFAULT_BUCKET_LEN)
      << "Assertion was to not re-allocate the Map instance.";

  EXPECT_STREQ(reinterpret_cast<const char*>(MapGet(&map, "key1")), "value1");
  EXPECT_STREQ(reinterpret_cast<const char*>(MapGet(&map, "key2")), "value2");
  EXPECT_STREQ(reinterpret_cast<const char*>(MapGet(&map, "key3")), "value3");
  EXPECT_STREQ(reinterpret_cast<const char*>(MapGet(&map, "key4")), "value4");
  EXPECT_STREQ(reinterpret_cast<const char*>(MapGet(&map, "key5")), "value5");

  MapFree(&map);
}

class MapTest : public testing::Test {
 protected:
  void TearDown() override { MapFree(&map); }

 protected:
  Map map;
};

TEST_F(MapTest, TestWhenAllocatedWithMapAllocStrAsKey) {
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
