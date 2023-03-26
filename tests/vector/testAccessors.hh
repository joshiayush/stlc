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

#ifndef CJSON_TESTS_VECTOR_TESTACCESSORS_HH_
#define CJSON_TESTS_VECTOR_TESTACCESSORS_HH_

#include <gtest/gtest.h>

#include <cstdint>
#include <cstdlib>

#include "utils.hh"
#include "vector/vector.h"

class VectorAccessorsTest : public ::testing::Test {
 protected:
  void TearDown() override {
    if (vector.data != nullptr) { VectorFree(&vector); }
  }

 protected:
  Vector vector;
};

class VectorSetTest : public VectorAccessorsTest {};

// This test must not raise "segmentation fault" error as giving `NULL` pointer
// as a vector instance must make the function `VectorSet()` immediately return
// without de-referencing anything.
TEST_F(VectorSetTest, WhenVectorInstanceIsNull) {
  VectorSet(NULL, 0, 0);
  EXPECT_TRUE(true);
}

TEST_F(VectorSetTest, WhenElementIndexIsGreaterThanVectorSize) {
  VectorInit(&vector, -1);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity = 0;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  size_t* element = (size_t*)std::malloc(sizeof(size_t) * 1);
  *element = 10;
  ASSERT_EQ(vector.capacity, capacity);
  VectorSet(&vector, element, vector.size + 1);
  std::free(element);
}

TEST_F(VectorSetTest, WhenIndexIsInBounds) {
  VectorInit(&vector, -1);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity = 0;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  size_t* element = (size_t*)std::malloc(sizeof(size_t) * 1);
  *element = 10;
  VectorPush(&vector, element);
  EXPECT_EQ(*(size_t*)VectorGet(&vector, 0), *element);
  *element = 20;
  VectorSet(&vector, element, 0);
  EXPECT_EQ(*(size_t*)VectorGet(&vector, 0), *element);
  std::free(element);
}

class VectorGetTest : public VectorAccessorsTest {};

TEST_F(VectorGetTest, WhenVectorInstanceIsNull) {
  EXPECT_EQ((nullptr_t*)VectorGet(NULL, 0), nullptr);
}

TEST_F(VectorGetTest, WhenElementIndexIsGreaterThanVectorSize) {
  VectorInit(&vector, -1);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity = 0;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  size_t* element = (size_t*)std::malloc(sizeof(size_t) * 1);
  *element = 10;
  ASSERT_EQ(vector.capacity, capacity);
  VectorSet(&vector, element, vector.size + 1);
  std::free(element);
  EXPECT_EQ((nullptr_t*)VectorGet(&vector, vector.size + 1), nullptr);
}

TEST_F(VectorGetTest, WhenIndexIsInBounds) {
  VectorInit(&vector, -1);
  ASSERT_NE(vector.data, nullptr);
  ASSERT_EQ(vector.size, 0);
  size_t capacity = 0;
  cjson::testing::vector::utils::ComputeVectorBufferCapacity(
      VECTOR_DEFAULT_SIZE, capacity);
  size_t* element = (size_t*)std::malloc(sizeof(size_t) * 1);
  *element = 10;
  VectorPush(&vector, element);
  EXPECT_EQ(*(size_t*)VectorGet(&vector, 0), *element);
  std::free(element);
}

#endif  // CJSON_TESTS_VECTOR_TESTACCESSORS_HH_
