// Copyright 2021, joshiayush Inc.
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
//     * Neither the name of joshiayush Inc. nor the names of its
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

#include "data/vector/vector.h"

#define _ARBITRARY_VECTOR_TESTING_LENGTH 2147489

class VectorTest : public ::testing::Test {
 protected:
  // Computes the capacity of the ``Vector`` instance using the Python list
  // resize routine that is identical to static function
  // ``_ComputeVectorBufferCapacity()`` inside module ``sstream.c``.
  void _ComputeVectorBufferCapacity(const size_t& size, size_t& capacity) {
    capacity = (size >> 3) + (size < 9 ? 3 : 6);
    capacity += size;
  }

  void TearDown() override { VectorFree(&vector); }

 protected:
  Vector vector;
};

TEST_F(VectorTest, VectorDefAllocFunctionWithDefaultLengthTest) {
  vector = VectorDefAlloc();
  ASSERT_NE(vector.data, (void*)0);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  _ComputeVectorBufferCapacity(VECTOR_DEFAULT_SIZE, capacity);
  ASSERT_EQ(vector.capacity, capacity);
}

TEST_F(VectorTest, VectorAllocFunctionWithArbitraryVectorLengthTest) {
  vector = VectorAlloc(_ARBITRARY_VECTOR_TESTING_LENGTH);
  ASSERT_NE(vector.data, (void*)0);
  ASSERT_EQ(vector.size, 0);
  size_t capacity;
  _ComputeVectorBufferCapacity(_ARBITRARY_VECTOR_TESTING_LENGTH, capacity);
  ASSERT_EQ(vector.capacity, capacity);
}

#endif  // CJSON_TESTS_VECTOR_TESTVECTOR_HH_
