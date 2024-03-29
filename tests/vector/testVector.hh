// Copyright 2021, The stlc authors.
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
//     * Neither the name of The stlc authors. nor the names of its
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

#ifndef STLC_TESTS_VECTOR_TESTVECTOR_HH_
#define STLC_TESTS_VECTOR_TESTVECTOR_HH_

#include <gtest/gtest.h>

#include <cstdlib>

#include "utils.hh"
#include "vector/vector.h"

#define _ARBITRARY_VECTOR_TESTING_LENGTH 2147489

class VectorTest : public ::testing::Test {
 protected:
  void TearDown() override {
    if (vector.data != nullptr) VectorFree(&vector);
  }

 protected:
  Vector vector;
};

class VectorInitTest : public VectorTest {};

TEST_F(VectorInitTest, WhenAllocatedWithDefaultSize) {
  VectorInit(&vector, -1);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  ASSERT_EQ(vector.capacity, capacity);
}

TEST_F(VectorInitTest, WhenAllocatedWithAllocAndZeroAsSize) {
  VectorInit(&vector, 0);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(0, capacity);
  // Even a size of zero should result in a capacity equals to 3.
  ASSERT_EQ(vector.capacity, capacity);
}

TEST_F(VectorInitTest, WhenAllocatedWithAllocAndAArbitrarySize) {
  VectorInit(&vector, _ARBITRARY_VECTOR_TESTING_LENGTH);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(
      _ARBITRARY_VECTOR_TESTING_LENGTH, capacity);
  ASSERT_EQ(vector.capacity, capacity);
}

class VectorResizeTest : public VectorTest {};

TEST_F(VectorResizeTest, WhenZeroIsUsedAsSize) {
  VectorInit(&vector, 0);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(0, capacity);
  // Even a size of zero should result in a capacity equals to 3.
  ASSERT_EQ(vector.capacity, capacity);
  // A size of zero still results in a capacity of 3 so `VectorResize()` must
  // return `VECTOR_RESIZE_NOT_REQUIRED` when called with a size of zero.
  ASSERT_EQ(VectorResize(&vector, 0), VECTOR_RESIZE_NOT_REQUIRED);
}

TEST_F(VectorResizeTest, WhenAArbitraryNumberIsUsedAsSizeForVectorResize) {
  VectorInit(&vector, _ARBITRARY_VECTOR_TESTING_LENGTH);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(
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
  VectorInit(&vector, _ARBITRARY_VECTOR_TESTING_LENGTH);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(
      _ARBITRARY_VECTOR_TESTING_LENGTH, capacity);
  ASSERT_EQ(vector.capacity, capacity);
  ASSERT_EQ(VectorResize(&vector, _ARBITRARY_VECTOR_TESTING_LENGTH * 2),
            VECTOR_RESIZE_SUCCESS);
}

class VectorCopyTest : public VectorTest {};

TEST_F(VectorCopyTest, WhenZeroIsUsedAsSrcSize) {
  VectorInit(&vector, 0);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(0, capacity);
  // Even a size of zero should result in a capacity equals to 3.
  ASSERT_EQ(vector.capacity, capacity);
  Vector dest;
  VectorInit(&dest, 0);
  ASSERT_NE(dest.data, nullptr);
  ASSERT_EQ(dest.size, 0);
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(0, capacity);
  // Even a size of zero should result in a capacity equals to 3.
  ASSERT_EQ(dest.capacity, capacity);
  ASSERT_EQ(VectorCopy(&dest, &vector), VECTOR_COPY_SUCCESS);
  VectorFree(&dest);
}

TEST_F(VectorCopyTest, WhenArbitraryNumbersAreUsedAsElementsToSrcVector) {
  size_t vector_size = 10;
  VectorInit(&vector, vector_size);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(vector_size,
                                                             capacity);
  ASSERT_EQ(vector.capacity, capacity);
  size_t* array = (size_t*)std::malloc(sizeof(size_t) * vector_size);
  for (size_t i = 0; i < vector_size; ++i) array[i] = i;
  for (size_t i = 0; i < vector_size; ++i) VectorPush(&vector, &array[i]);
  Vector dest;
  VectorInit(&dest, -1);
  ASSERT_NE(dest.data, nullptr);
  ASSERT_EQ(dest.size, 0);
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  ASSERT_EQ(dest.capacity, capacity);
  ASSERT_EQ(VectorCopy(&dest, &vector), VECTOR_COPY_SUCCESS);
  ASSERT_EQ(dest.size, vector.size);
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(dest.size,
                                                             capacity);
  ASSERT_EQ(dest.capacity, capacity);
  for (size_t i = 0; i < dest.size; ++i)
    ASSERT_EQ(*(int*)(dest.data[i]), *(int*)(vector.data[i]));
  std::free(array);
  VectorFree(&dest);
}

class VectorClearTest : public VectorTest {};

TEST_F(VectorClearTest, WhenElementsAreNotStoredInTheFreeStore) {
  size_t vector_size = 10;
  VectorInit(&vector, vector_size);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(vector_size,
                                                             capacity);
  ASSERT_EQ(vector.capacity, capacity);
  size_t array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (size_t i = 0; i < vector_size; ++i) VectorPush(&vector, &array[i]);
  VectorClear(&vector);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(vector.size,
                                                             capacity);
  ASSERT_EQ(vector.capacity, capacity);

  // Check if the elements are still stored in the stack after calling
  // `VectorClear()`.
  for (size_t i = 0; i < vector_size; ++i) ASSERT_EQ(array[i], i);
}

TEST_F(VectorClearTest, WhenElementsAreStoredInTheFreeStore) {
  size_t vector_size = 10;
  VectorInit(&vector, vector_size);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(vector_size,
                                                             capacity);
  ASSERT_EQ(vector.capacity, capacity);
  size_t* array = (size_t*)std::malloc(sizeof(size_t) * vector_size);
  for (size_t i = 0; i < vector_size; ++i) array[i] = i;
  for (size_t i = 0; i < vector_size; ++i) VectorPush(&vector, &array[i]);
  VectorClear(&vector);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(vector.size,
                                                             capacity);
  ASSERT_EQ(vector.capacity, capacity);

  // Check if the elements are still stored in the free store after calling
  // `VectorClear()`.
  for (size_t i = 0; i < vector_size; ++i) ASSERT_EQ(array[i], i);
  std::free(array);
}

class VectorFreeTest : public VectorTest {};

TEST_F(VectorFreeTest, WhenVectorIsAllocatedUsingDefaultSize) {
  VectorInit(&vector, -1);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  ASSERT_EQ(vector.capacity, capacity);
  VectorFree(&vector);
  ASSERT_EQ(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  ASSERT_EQ(vector.capacity, 0);
}

class VectorFreeDeepTest : public VectorTest {};

TEST_F(VectorFreeDeepTest, WhenVectorIsCopiedFromADynamicallyAllocatedArray) {
  size_t vector_size = 10;
  VectorInit(&vector, vector_size);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(vector_size,
                                                             capacity);
  ASSERT_EQ(vector.capacity, capacity);
  for (size_t i = 0; i < vector_size; ++i) {
    size_t* element = (size_t*)malloc(sizeof(size_t) * 1);
    *element = i;
    VectorPush(&vector, element);
  }
  VectorFreeDeep(&vector);
  ASSERT_EQ(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  ASSERT_EQ(vector.capacity, 0);
}

#endif  // STLC_TESTS_VECTOR_TESTVECTOR_HH_
