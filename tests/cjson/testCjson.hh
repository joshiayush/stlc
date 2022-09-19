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

#ifndef CJSON_TESTS_CJSON_TESTCJSON_HH_
#define CJSON_TESTS_CJSON_TESTCJSON_HH_

#include <gtest/gtest.h>

#include <cstdio>
#include <limits>

#include "../map/utils.hh"
#include "../vector/utils.hh"
#include "bool.h"
#include "bytes.h"
#include "cjson.h"
#include "data/map/map.h"
#include "data/vector/vector.h"

TEST(JSON_InitTypeTest, TestWhenJSON_NullIsUsed) {
  JSON json = JSON_INIT_TYPE(Null);

  EXPECT_EQ(json.type, JSON_Null);
  EXPECT_EQ(json.value.null, nullptr);
}

TEST(JSON_InitTypeTest, TestWhenJSON_BooleanIsUsed) {
  JSON json = JSON_INIT_TYPE(Boolean);

  EXPECT_EQ(json.type, JSON_Boolean);
  EXPECT_EQ(json.value.boolean, false);
}

TEST(JSON_InitTypeTest, TestWhenJSON_NumberIsUsed) {
  JSON json = JSON_INIT_TYPE(Number);

  EXPECT_EQ(json.type, JSON_Number);
  EXPECT_EQ(json.value.number, 0);
}

TEST(JSON_InitTypeTest, TestWhenJSON_DecimalIsUsed) {
  JSON json = JSON_INIT_TYPE(Decimal);

  EXPECT_EQ(json.type, JSON_Decimal);
  EXPECT_EQ(json.value.decimal, 0.0);
}

TEST(JSON_InitTypeTest, TestWhenJSON_StringIsUsed) {
  JSON json = JSON_INIT_TYPE(String);

  EXPECT_EQ(json.type, JSON_String);
  EXPECT_EQ(*json.value.string, '\0');
}

TEST(JSON_InitTypeTest, TestWhenJSON_ListIsUsedWithNoCapacityGiven) {
  JSON json = JSON_INIT_TYPE(List);

  EXPECT_EQ(json.type, JSON_List);
  EXPECT_NE(json.value.list.data, nullptr);
  EXPECT_EQ(json.value.list.size, 0);

  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(0, capacity);
  EXPECT_EQ(json.value.list.capacity, capacity);
}

TEST(JSON_InitTypeTest, TestWhenJSON_ListIsUsedWithCapacityGiven) {
  JSON json = JSON_INIT_TYPE_SIZE(List, 10);

  EXPECT_EQ(json.type, JSON_List);
  EXPECT_NE(json.value.list.data, nullptr);
  EXPECT_EQ(json.value.list.size, 0);

  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(10, capacity);
  EXPECT_EQ(json.value.list.capacity, capacity);
}

TEST(JSON_InitTypeTest, TestWhenJSON_ObjectIsUsedWithNoEntriesGiven) {
  JSON json = JSON_INIT_TYPE(Object);

  EXPECT_EQ(json.type, JSON_Object);
  EXPECT_NE(json.value.object.buckets, nullptr);
  EXPECT_EQ(json.value.object.bucketslen, MAP_DEFAULT_BUCKET_LEN);
  EXPECT_EQ(json.value.object.entrieslen, 0);
  EXPECT_EQ(json.value.object.hash, Hash);
  EXPECT_EQ(json.value.object.keycmp, KeyCmp);
}

TEST(JSON_InitTypeTest, TestWhenJSON_ObjectIsUsedWhenLessEntriesAreGiven) {
  JSON json = JSON_INIT_TYPE_SIZE(Object, 10);

  EXPECT_EQ(json.type, JSON_Object);
  EXPECT_NE(json.value.object.buckets, nullptr);
  EXPECT_EQ(json.value.object.bucketslen, MAP_DEFAULT_BUCKET_LEN);
  EXPECT_EQ(json.value.object.entrieslen, 0);
  EXPECT_EQ(json.value.object.hash, Hash);
  EXPECT_EQ(json.value.object.keycmp, KeyCmp);
}

TEST(JSON_InitTypeTest, TestWhenJSON_ObjectIsUsedWhenHighEntriesAreGiven) {
  JSON json = JSON_INIT_TYPE_SIZE(Object, 20);

  EXPECT_EQ(json.type, JSON_Object);
  EXPECT_NE(json.value.object.buckets, nullptr);
  EXPECT_EQ(json.value.object.bucketslen, 32);
  EXPECT_EQ(json.value.object.entrieslen, 0);
  EXPECT_EQ(json.value.object.hash, Hash);
  EXPECT_EQ(json.value.object.keycmp, KeyCmp);
}

TEST(JSON_InitTypeSizeTest, TestWhenJSON_NullIsUsed) {
  JSON json = JSON_INIT_TYPE_SIZE(Null, 0);

  EXPECT_EQ(json.type, JSON_Null);
  EXPECT_EQ(json.value.null, nullptr);
}

TEST(JSON_InitTypeSizeTest, TestWhenJSON_BooleanIsUsed) {
  JSON json = JSON_INIT_TYPE_SIZE(Boolean, 0);

  EXPECT_EQ(json.type, JSON_Boolean);
  EXPECT_EQ(json.value.boolean, false);
}

TEST(JSON_InitTypeSizeTest, TestWhenJSON_NumberIsUsed) {
  JSON json = JSON_INIT_TYPE_SIZE(Number, 0);

  EXPECT_EQ(json.type, JSON_Number);
  EXPECT_EQ(json.value.number, 0);
}

TEST(JSON_InitTypeSizeTest, TestWhenJSON_DecimalIsUsed) {
  JSON json = JSON_INIT_TYPE_SIZE(Decimal, 0);

  EXPECT_EQ(json.type, JSON_Decimal);
  EXPECT_EQ(json.value.decimal, 0.0);
}

TEST(JSON_InitTypeSizeTest, TestWhenJSON_StringIsUsed) {
  JSON json = JSON_INIT_TYPE_SIZE(String, 0);

  EXPECT_EQ(json.type, JSON_String);
  EXPECT_EQ(*json.value.string, '\0');
}

TEST(JSON_InitTypeSizeTest, TestWhenJSON_ListIsUsedWhenCapacityZeroIsGiven) {
  JSON json = JSON_INIT_TYPE_SIZE(List, 0);

  EXPECT_EQ(json.type, JSON_List);
  EXPECT_NE(json.value.list.data, nullptr);
  EXPECT_EQ(json.value.list.size, 0);

  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(0, capacity);
  EXPECT_EQ(json.value.list.capacity, capacity);
}

TEST(JSON_InitTypeSizeTest, TestWhenJSON_ListIsUsedWhenCapacityTenIsGiven) {
  JSON json = JSON_INIT_TYPE_SIZE(List, 10);

  EXPECT_EQ(json.type, JSON_List);
  EXPECT_NE(json.value.list.data, nullptr);
  EXPECT_EQ(json.value.list.size, 0);

  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(10, capacity);
  EXPECT_EQ(json.value.list.capacity, capacity);
}

TEST(JSON_InitTypeSizeTest, TestWhenJSON_ObjectIsUsedWhenZeroEntriesAreGiven) {
  JSON json = JSON_INIT_TYPE_SIZE(Object, 0);

  EXPECT_EQ(json.type, JSON_Object);
  EXPECT_NE(json.value.object.buckets, nullptr);
  EXPECT_EQ(json.value.object.bucketslen, MAP_DEFAULT_BUCKET_LEN);
  EXPECT_EQ(json.value.object.entrieslen, 0);
  EXPECT_EQ(json.value.object.hash, Hash);
  EXPECT_EQ(json.value.object.keycmp, KeyCmp);
}

TEST(JSON_InitTypeSizeTest, TestWhenJSON_ObjectIsUsedWhenTenEntriesAreGiven) {
  JSON json = JSON_INIT_TYPE_SIZE(Object, 10);

  EXPECT_EQ(json.type, JSON_Object);
  EXPECT_NE(json.value.object.buckets, nullptr);
  EXPECT_EQ(json.value.object.bucketslen, MAP_DEFAULT_BUCKET_LEN);
  EXPECT_EQ(json.value.object.entrieslen, 0);
  EXPECT_EQ(json.value.object.hash, Hash);
  EXPECT_EQ(json.value.object.keycmp, KeyCmp);
}

TEST(JSON_InitTypeSizeTest, TestWhenJSON_ObjectIsUsedWhenHighEntriesAreGiven) {
  JSON json = JSON_INIT_TYPE_SIZE(Object, 20);

  EXPECT_EQ(json.type, JSON_Object);
  EXPECT_NE(json.value.object.buckets, nullptr);
  EXPECT_EQ(json.value.object.bucketslen, 32);
  EXPECT_EQ(json.value.object.entrieslen, 0);
  EXPECT_EQ(json.value.object.hash, Hash);
  EXPECT_EQ(json.value.object.keycmp, KeyCmp);
}

TEST(JSON_InitNullImplTest, TestFunctionJSON_InitNullImpl) {
  JSON json = JSON_InitNullImpl();

  EXPECT_EQ(json.type, JSON_Null);
  EXPECT_EQ(json.value.null, nullptr);
}

TEST(JSON_InitStringImplTest, TestWhenStringIsEmpty) {
  JSON json = JSON_InitStringImpl(JSON_CONST_STRINGIFY(""));

  EXPECT_EQ(json.type, JSON_String);
  EXPECT_EQ(*json.value.string, '\0');
}

TEST(JSON_InitStringImplTest, TestWhenStringIsNotEmpty) {
  JSON json = JSON_InitStringImpl(JSON_CONST_STRINGIFY("foo"));

  EXPECT_EQ(json.type, JSON_String);
  EXPECT_STREQ(json.value.string, "foo");
}

TEST(JSON_InitNumberImplTest, TestWhenINT64_MINIsUsed) {
  JSON json = JSON_InitNumberImpl(INT64_MIN);

  EXPECT_EQ(json.type, JSON_Number);
  EXPECT_EQ(json.value.number, INT64_MIN);
}

TEST(JSON_InitNumberImplTest, TestWhenINT64_MAXIsGiven) {
  JSON json = JSON_InitNumberImpl(INT64_MAX);

  EXPECT_EQ(json.type, JSON_Number);
  EXPECT_EQ(json.value.number, INT64_MAX);
}

TEST(JSON_InitDecimalImplTest, TestWhenDBL_MINIsGiven) {
  JSON json = JSON_InitDecimalImpl(DBL_MIN);

  EXPECT_EQ(json.type, JSON_Decimal);
  EXPECT_EQ(json.value.decimal, DBL_MIN);
}

TEST(JSON_InitDecimalImplTest, TestWhenDBL_MAXIsGiven) {
  JSON json = JSON_InitDecimalImpl(DBL_MAX);

  EXPECT_EQ(json.type, JSON_Decimal);
  EXPECT_EQ(json.value.decimal, DBL_MAX);
}

TEST(JSON_InitBooleanImplTest, TestWhenTRUEIsGiven) {
  JSON json = JSON_InitBoolImpl(TRUE);

  EXPECT_EQ(json.type, JSON_Boolean);
  EXPECT_EQ(json.value.boolean, TRUE);
}

TEST(JSON_InitBooleanImplTest, TestWhenFALSEIsGiven) {
  JSON json = JSON_InitBoolImpl(FALSE);

  EXPECT_EQ(json.type, JSON_Boolean);
  EXPECT_EQ(json.value.boolean, FALSE);
}

TEST(JSON_InitListImplTest, TestWhenNullIsGiven) {
  JSON json = JSON_InitListImpl(nullptr);

  EXPECT_EQ(json.type, JSON_List);
  EXPECT_EQ(json.value.list.data, nullptr);
  EXPECT_EQ(json.value.list.size, 0);

  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(0, capacity);
  EXPECT_EQ(json.value.list.capacity, capacity);
}

TEST(JSON_InitListImplTest, TestWhenAEmptyVectorInstanceIsGiven) {
  Vector vec = VectorDefAlloc();
  JSON json = JSON_InitListImpl(&vec);

  EXPECT_EQ(json.type, JSON_List);
  EXPECT_NE(json.value.list.data, nullptr);
  EXPECT_EQ(json.value.list.size, 0);

  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(0, capacity);
  EXPECT_EQ(json.value.list.capacity, capacity);
}

TEST(JSON_InitListImplTest, TestWhenAVectorInstanceIsGiven) {
  Vector vec = VectorDefAlloc();

  int64_t value1 = 1;
  int64_t value2 = 2;
  int64_t value3 = 3;

  VectorPush(&vec, &value1);
  VectorPush(&vec, &value2);
  VectorPush(&vec, &value3);

  JSON json = JSON_InitListImpl(&vec);

  EXPECT_EQ(json.type, JSON_List);
  EXPECT_NE(json.value.list.data, nullptr);
  for (size_t i = 0; i < json.value.list.size; ++i)
    EXPECT_EQ(*(const int*)VectorGet(&json.value.list, i),
              *(const int*)VectorGet(&vec, i))
        << "At index: " << i << "\n  json.value.list[" << i << "]("
        << *(const int*)VectorGet(&json.value.list, i) << ") != vec[" << i
        << "](" << *(const int*)VectorGet(&vec, i) << ")";
  EXPECT_EQ(json.value.list.size, 3);

  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(0, capacity);
  EXPECT_EQ(json.value.list.capacity, capacity);
}

TEST(JSON_InitObjectImplTest, TestWhenNullIsGiven) {
  JSON json = JSON_InitObjectImpl(nullptr);

  EXPECT_EQ(json.type, JSON_Object);
  EXPECT_EQ(json.value.object.buckets, nullptr);
  EXPECT_EQ(json.value.object.bucketslen, MAP_DEFAULT_BUCKET_LEN);
  EXPECT_EQ(json.value.object.entrieslen, 0);
}

TEST(JSON_InitObjectImplTest, TestWhenAEmptyMapInstanceIsGiven) {
  Map map = MapAlloc(Hash, KeyCmp);
  JSON json = JSON_InitObjectImpl(&map);

  EXPECT_EQ(json.type, JSON_Object);
  EXPECT_NE(json.value.object.buckets, nullptr);
  EXPECT_EQ(json.value.object.bucketslen, MAP_DEFAULT_BUCKET_LEN);
  EXPECT_EQ(json.value.object.entrieslen, 0);
  EXPECT_EQ(json.value.object.hash, Hash);
  EXPECT_EQ(json.value.object.keycmp, KeyCmp);

  MapFree(&map);
}

TEST(JSON_InitObjectImplTest, TestWhenAMapInstanceIsGiven) {
  Map map = MapAllocNBuckets(16, Hash, KeyCmp);
  JSON json = JSON_InitObjectImpl(&map);

  EXPECT_EQ(json.type, JSON_Object);
  EXPECT_NE(json.value.object.buckets, nullptr);
  EXPECT_EQ(json.value.object.bucketslen, 16);
  EXPECT_EQ(json.value.object.entrieslen, 0);
  EXPECT_EQ(json.value.object.hash, Hash);
  EXPECT_EQ(json.value.object.keycmp, KeyCmp);

  MapFree(&map);
}

TEST(JSON_InitObjectImplTest, TestWhenAMapInstanceIsAllocatedUsingEntries) {
  Map map = MapAllocNEntries(12UL, Hash, KeyCmp);
  JSON json = JSON_InitObjectImpl(&map);

  EXPECT_EQ(json.type, JSON_Object);
  EXPECT_NE(json.value.object.buckets, nullptr);
  EXPECT_EQ(json.value.object.bucketslen,
            cjson::testing::map::utils::ComputeBucketsLenSatisfyingLoadFactor(
                MAX_LOAD_FACTOR, 12UL));
  EXPECT_EQ(json.value.object.entrieslen, 0);
  EXPECT_EQ(json.value.object.hash, Hash);
  EXPECT_EQ(json.value.object.keycmp, KeyCmp);

  MapFree(&map);
}

TEST(JSON_InitObjectImplTest, TestWhenAMapInstanceWithValuesIsGiven) {
  Map map = MapAlloc(Hash, KeyCmp);

  EXPECT_EQ(map.entrieslen, 0);
  EXPECT_EQ(map.bucketslen, MAP_DEFAULT_BUCKET_LEN);
  EXPECT_EQ(map.hash, Hash);
  EXPECT_EQ(map.keycmp, KeyCmp);

  MapPut(&map, "key1", "value1");
  MapPut(&map, "key2", "value2");
  MapPut(&map, "key3", "value3");

  ASSERT_EQ(map.entrieslen, 3)
      << "Entries length didn't change. Continuing will lead to failure!";

  JSON json = JSON_InitObjectImpl(&map);

  EXPECT_EQ(json.type, JSON_Object);

  EXPECT_STREQ(
      reinterpret_cast<const char*>(MapGet(&json.value.object, "key1")),
      "value1");
  EXPECT_STREQ(
      reinterpret_cast<const char*>(MapGet(&json.value.object, "key2")),
      "value2");
  EXPECT_STREQ(
      reinterpret_cast<const char*>(MapGet(&json.value.object, "key3")),
      "value3");

  MapFree(&map);
}

TEST(JSON_AllocTypeTest, TestWhenJSON_NullIsUsed) {
  JSON* json = JSON_ALLOC_TYPE(Null);

  EXPECT_EQ(json->type, JSON_Null);
  EXPECT_EQ(json->value.null, nullptr);

  JSON_Free(json);
}

TEST(JSON_AllocTypeTest, TestWhenJSON_BooleanIsUsed) {
  JSON* json = JSON_ALLOC_TYPE(Boolean);

  EXPECT_EQ(json->type, JSON_Boolean);
  EXPECT_EQ(json->value.boolean, false);

  JSON_Free(json);
}

TEST(JSON_AllocTypeTest, TestWhenJSON_NumberIsUsed) {
  JSON* json = JSON_ALLOC_TYPE(Number);

  EXPECT_EQ(json->type, JSON_Number);
  EXPECT_EQ(json->value.number, 0);

  JSON_Free(json);
}

TEST(JSON_AllocTypeTest, TestWhenJSON_DecimalIsUsed) {
  JSON* json = JSON_ALLOC_TYPE(Decimal);

  EXPECT_EQ(json->type, JSON_Decimal);
  EXPECT_EQ(json->value.decimal, 0.0);

  JSON_Free(json);
}

TEST(JSON_AllocTypeTest, TestWhenJSON_StringIsUsed) {
  JSON* json = JSON_ALLOC_TYPE(String);

  EXPECT_EQ(json->type, JSON_String);
  EXPECT_NE(json->value.string, nullptr);

  JSON_Free(json);
}

TEST(JSON_AllocTypeTest, TestWhenJSON_ListIsUsedWithNoCapacityGiven) {
  JSON* json = JSON_ALLOC_TYPE(List);

  EXPECT_EQ(json->type, JSON_List);
  EXPECT_NE(json->value.list.data, nullptr);
  EXPECT_EQ(json->value.list.size, 0);

  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(sizeof(JSON),
                                                             capacity);
  EXPECT_EQ(json->value.list.capacity, capacity);

  JSON_Free(json);
}

TEST(JSON_AllocTypeTest, TestWhenJSON_ListIsUsedWithCapacityGiven) {
  const size_t type_size = 10UL;
  JSON* json = JSON_ALLOC_TYPE_SIZE(List, type_size);

  EXPECT_EQ(json->type, JSON_List);
  EXPECT_NE(json->value.list.data, nullptr);
  EXPECT_EQ(json->value.list.size, 0);

  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(type_size,
                                                             capacity);
  EXPECT_EQ(json->value.list.capacity, capacity);

  JSON_Free(json);
}

TEST(JSON_AllocTypeTest, TestWhenJSON_ObjectIsUsedWithEntriesAsSizeOfJSON) {
  JSON* json = JSON_ALLOC_TYPE(Object);

  EXPECT_EQ(json->type, JSON_Object);
  EXPECT_NE(json->value.object.buckets, nullptr);
  EXPECT_EQ(json->value.object.bucketslen,
            cjson::testing::map::utils::ComputeBucketsLenSatisfyingLoadFactor(
                MAX_LOAD_FACTOR, sizeof(JSON)));
  EXPECT_EQ(json->value.object.entrieslen, 0);
  EXPECT_EQ(json->value.object.hash, Hash);
  EXPECT_EQ(json->value.object.keycmp, KeyCmp);

  JSON_Free(json);
}

TEST(JSON_AllocTypeSizeTest, TestWhenZeroSizeIsGiven) {
  JSON* json = JSON_ALLOC_TYPE_SIZE(Object, 0);

  if (json == nullptr) {
    GTEST_LOG_(INFO) << "For size 0 malloc returned (nullptr).\nEarly return.";
    return;
  }

  GTEST_LOG_(INFO) << "For size 0 malloc returned unique pointer.";

  EXPECT_EQ(json->type, JSON_Object);
  EXPECT_NE(json->value.object.buckets, nullptr);
  EXPECT_EQ(json->value.object.bucketslen,
            cjson::testing::map::utils::ComputeBucketsLenSatisfyingLoadFactor(
                MAX_LOAD_FACTOR, 0));
  EXPECT_EQ(json->value.object.entrieslen, 0);
  EXPECT_EQ(json->value.object.hash, Hash);
  EXPECT_EQ(json->value.object.keycmp, KeyCmp);

  JSON_Free(json);
}

TEST(JSON_AllocTypeSizeTest, TestWhenArbitrarySizeForStringTypeIsGiven) {
  const size_t type_size = sizeof(JSON);
  JSON* json = JSON_ALLOC_TYPE_SIZE(String, type_size);

  EXPECT_EQ(json->type, JSON_String);
  EXPECT_NE(json->value.string, nullptr);

  JSON_Free(json);
}

TEST(JSON_AllocTypeSizeTest, TestWhenArbitrarySizeForListTypeIsGiven) {
  const size_t type_size = sizeof(JSON);
  JSON* json = JSON_ALLOC_TYPE_SIZE(List, type_size);

  EXPECT_EQ(json->type, JSON_List);
  EXPECT_NE(json->value.list.data, nullptr);
  EXPECT_EQ(json->value.list.size, 0);

  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(type_size,
                                                             capacity);
  EXPECT_EQ(json->value.list.capacity, capacity);

  JSON_Free(json);
}

TEST(JSON_AllocTypeSizeTest, TestWhenArbitrarySizeForObjectTypeIsGiven) {
  const size_t type_size = sizeof(JSON);
  JSON* json = JSON_ALLOC_TYPE_SIZE(Object, type_size);

  EXPECT_EQ(json->type, JSON_Object);
  EXPECT_NE(json->value.object.buckets, nullptr);
  EXPECT_EQ(json->value.object.bucketslen,
            cjson::testing::map::utils::ComputeBucketsLenSatisfyingLoadFactor(
                MAX_LOAD_FACTOR, type_size));
  EXPECT_EQ(json->value.object.entrieslen, 0);
  EXPECT_EQ(json->value.object.hash, Hash);
  EXPECT_EQ(json->value.object.keycmp, KeyCmp);

  JSON_Free(json);
}

#endif
