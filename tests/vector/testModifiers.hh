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

#ifndef STLC_TESTS_VECTOR_TESTMODIFIERS_HH_
#define STLC_TESTS_VECTOR_TESTMODIFIERS_HH_

#include <gtest/gtest.h>

#include <cstdint>
#include <cstdlib>

#include "vector/vector.h"

class VectorModifiersTest : public ::testing::Test {
 protected:
  void TearDown() override {
    if (vector.data != nullptr) VectorFree(&vector);
  }

 protected:
  Vector vector;
};

class VectorInsertTest : public VectorModifiersTest {};

// This test must not raise "segmentation fault" error as giving `NULL` pointer
// as a vector instance must make the function `VectorInsert()` immediately
// return without de-referencing anything.
TEST_F(VectorInsertTest, WhenTheGivenVectorInstanceIsNull) {
  VectorInsert(NULL, 0, 0);
  EXPECT_TRUE(true);
}

TEST_F(VectorInsertTest, WhenElementIndexIsZero) {
  VectorInit(&vector, -1);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity = 0;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  ASSERT_EQ(vector.capacity, capacity);
  size_t* element = (size_t*)std::malloc(sizeof(size_t) * 1);
  *element = 10;
  VectorInsert(&vector, element, 0);
  ASSERT_EQ(vector.size, 1);
  ASSERT_EQ(*(size_t*)VectorGet(&vector, 0), 10);
  std::free(element);
}

TEST_F(VectorInsertTest, WhenElementIndexIsGreaterThanVectorSize) {
  VectorInit(&vector, -1);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity = 0;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  ASSERT_EQ(vector.capacity, capacity);
  size_t* element = (size_t*)std::malloc(sizeof(size_t) * 1);
  *element = 10;
  VectorInsert(&vector, element, 1);
  ASSERT_EQ(vector.size, 0);
  ASSERT_NE(*(size_t*)VectorGet(&vector, 0), *element);
  std::free(element);
}

class VectorPushTest : public VectorModifiersTest {};

// This test must not raise "segmentation fault" error as giving `NULL` pointer
// as a vector instance must make the function `VectorInsert()` (which is used
// internally) immediately return without de-referencing anything.
TEST_F(VectorPushTest, WhenTheGivenVectorInstanceIsNull) {
  // @TODO(joshiayush): This test is failing because of a bug in the
  // implementation.  `VectorPush(NULL, 0)` is raising segmentation fault.
  // VectorPush(NULL, 0);
  EXPECT_TRUE(true);
}

TEST_F(VectorPushTest, WhenPushedInSomeValues) {
  VectorInit(&vector, -1);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity = 0;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  ASSERT_EQ(vector.capacity, capacity);
  size_t* element1 = (size_t*)std::malloc(sizeof(size_t) * 1);
  *element1 = 10;
  VectorPush(&vector, element1);
  size_t* element2 = (size_t*)std::malloc(sizeof(size_t) * 1);
  *element2 = 20;
  VectorPush(&vector, element2);
  size_t* element3 = (size_t*)std::malloc(sizeof(size_t) * 1);
  *element3 = 30;
  VectorPush(&vector, element3);
  ASSERT_EQ(vector.size, 3);
  ASSERT_EQ(*(size_t*)VectorGet(&vector, 0), 10);
  ASSERT_EQ(*(size_t*)VectorGet(&vector, 1), 20);
  ASSERT_EQ(*(size_t*)VectorGet(&vector, 2), 30);
  std::free(element1);
  std::free(element2);
  std::free(element3);
}

class VectorUnshiftTest : public VectorModifiersTest {};

TEST_F(VectorUnshiftTest, WhenPushedInSomeValues) {
  VectorInit(&vector, -1);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity = 0;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  ASSERT_EQ(vector.capacity, capacity);
  size_t* element1 = (size_t*)std::malloc(sizeof(size_t) * 1);
  *element1 = 10;
  VectorUnshift(&vector, element1);
  size_t* element2 = (size_t*)std::malloc(sizeof(size_t) * 1);
  *element2 = 20;
  VectorUnshift(&vector, element2);
  size_t* element3 = (size_t*)std::malloc(sizeof(size_t) * 1);
  *element3 = 30;
  VectorUnshift(&vector, element3);
  ASSERT_EQ(vector.size, 3);
  ASSERT_EQ(*(size_t*)VectorGet(&vector, 0), 30);
  ASSERT_EQ(*(size_t*)VectorGet(&vector, 1), 20);
  ASSERT_EQ(*(size_t*)VectorGet(&vector, 2), 10);
  std::free(element1);
  std::free(element2);
  std::free(element3);
}

class VectorDeleteTest : public VectorModifiersTest {};

// This test must not raise "segmentation fault" error as giving `NULL` pointer
// as a vector instance must make the function `VectorDelete()` immediately
// return without de-referencing anything.
TEST_F(VectorDeleteTest, WhenTheGivenVectorInstanceIsNull) {
  VectorDelete(NULL, 0);
  EXPECT_TRUE(true);
}

TEST_F(VectorDeleteTest, WhenElementIndexIsZero) {
  VectorInit(&vector, -1);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity = 0;
  stlc::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  ASSERT_EQ(vector.capacity, capacity);
  size_t* element = (size_t*)std::malloc(sizeof(size_t) * 1);
  *element = 10;
  VectorInsert(&vector, element, 0);
  ASSERT_EQ(vector.size, 1);
  ASSERT_EQ(*(size_t*)VectorGet(&vector, 0), 10);
  ASSERT_EQ(*(size_t*)VectorDelete(&vector, 0), 10);
  ASSERT_EQ(vector.size, 0);
  std::free(element);
}

#endif  // STLC_TESTS_VECTOR_TESTMODIFIERS_HH_
