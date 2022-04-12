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

#ifndef CJSON_TESTS_VECTOR_TESTVECTOR_HH_
#define CJSON_TESTS_VECTOR_TESTVECTOR_HH_

#include <gtest/gtest.h>

#include <cstdlib>

#include "data/vector/vector.h"
#include "utils.hh"

#define _ARBITRARY_VECTOR_TESTING_LENGTH 2147489

class VectorTest : public ::testing::Test {
 protected:
  void TearDown() override {
    if (vector.data != nullptr)
      VectorFree(&vector);
  }

 protected:
  Vector vector;
};

class VectorDefAllocTest : public VectorTest {};

TEST_F(VectorDefAllocTest, WhenAllocatedWithDefAlloc) {
  vector = VectorDefAlloc();
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  ASSERT_EQ(vector.capacity, capacity);
}

class VectorAllocTest : public VectorTest {};

TEST_F(VectorAllocTest, WhenAllocatedWithAllocAndZeroAsSize) {
  vector = VectorAlloc(0);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(0, capacity);
  // Even a size of zero should result in a capacity equals to 3.
  ASSERT_EQ(vector.capacity, capacity);
}

TEST_F(VectorAllocTest, WhenAllocatedWithAllocAndAArbitrarySize) {
  vector = VectorAlloc(_ARBITRARY_VECTOR_TESTING_LENGTH);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(
      _ARBITRARY_VECTOR_TESTING_LENGTH, capacity);
  ASSERT_EQ(vector.capacity, capacity);
}

class VectorResizeTest : public VectorTest {};

TEST_F(VectorResizeTest, WhenZeroIsUsedAsSize) {
  vector = VectorAlloc(0);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(0, capacity);
  // Even a size of zero should result in a capacity equals to 3.
  ASSERT_EQ(vector.capacity, capacity);
  // A size of zero still results in a capacity of 3 so `VectorResize()` must
  // return `VECTOR_RESIZE_NOT_REQUIRED` when called with a size of zero.
  ASSERT_EQ(VectorResize(&vector, 0), VECTOR_RESIZE_NOT_REQUIRED);
}

TEST_F(VectorResizeTest, WhenAArbitraryNumberIsUsedAsSizeForVectorResize) {
  vector = VectorAlloc(_ARBITRARY_VECTOR_TESTING_LENGTH);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(
      _ARBITRARY_VECTOR_TESTING_LENGTH, capacity);
  ASSERT_EQ(vector.capacity, capacity);
  // In this test we test if `VectorResize()` returns
  // `VECTOR_RESIZE_NOT_REQUIRED` when vector's size and size to
  // `VectorResize()` are the same.
  ASSERT_EQ(VectorResize(&vector, _ARBITRARY_VECTOR_TESTING_LENGTH),
            VECTOR_RESIZE_NOT_REQUIRED);
}

TEST_F(VectorResizeTest,
       WhenDoubleOfArbitraryNumberIsUsedAsSizeForVectorResize) {
  vector = VectorAlloc(_ARBITRARY_VECTOR_TESTING_LENGTH);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(
      _ARBITRARY_VECTOR_TESTING_LENGTH, capacity);
  ASSERT_EQ(vector.capacity, capacity);
  ASSERT_EQ(VectorResize(&vector, _ARBITRARY_VECTOR_TESTING_LENGTH * 2),
            VECTOR_RESIZE_SUCCESS);
}

class VectorCopyTest : public VectorTest {};

TEST_F(VectorCopyTest, WhenZeroIsUsedAsSrcSize) {
  vector = VectorAlloc(0);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(0, capacity);
  // Even a size of zero should result in a capacity equals to 3.
  ASSERT_EQ(vector.capacity, capacity);
  Vector dest = VectorAlloc(0);
  ASSERT_NE(dest.data, nullptr);
  ASSERT_EQ(dest.size, 0);
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(0, capacity);
  // Even a size of zero should result in a capacity equals to 3.
  ASSERT_EQ(dest.capacity, capacity);
  ASSERT_EQ(VectorCopy(&dest, &vector), VECTOR_COPY_SUCCESS);
}

TEST_F(VectorCopyTest, WhenArbitraryNumbersAreUsedAsElementsToSrcVector) {
  size_t vector_size = 10;
  vector = VectorAlloc(vector_size);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(vector_size,
                                                             capacity);
  ASSERT_EQ(vector.capacity, capacity);
  size_t* array = (size_t*)std::malloc(sizeof(size_t) * vector_size);
  for (size_t i = 0; i < vector_size; i++)
    array[i] = i;
  for (size_t i = 0; i < vector_size; ++i)
    VectorPush(&vector, &array[i]);
  Vector dest = VectorDefAlloc();
  ASSERT_NE(dest.data, nullptr);
  ASSERT_EQ(dest.size, 0);
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  ASSERT_EQ(dest.capacity, capacity);
  ASSERT_EQ(VectorCopy(&dest, &vector), VECTOR_COPY_SUCCESS);
  for (size_t i = 0; i < dest.size; ++i)
    ASSERT_EQ(*(int*)(dest.data[i]), *(int*)(vector.data[i]));
  std::free(array);
  VectorFree(&dest);
}

#endif  // CJSON_TESTS_VECTOR_TESTVECTOR_HH_
