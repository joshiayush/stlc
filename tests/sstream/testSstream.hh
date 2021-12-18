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

#ifndef CJSON_TESTS_SSTREAM_TESTSSTREAM_HH_
#define CJSON_TESTS_SSTREAM_TESTSSTREAM_HH_

#include <gtest/gtest.h>

#include <cstring>

#include "data/sstream/accessors.h"
#include "data/sstream/sstream.h"

#define _ARBITRARY_TESTING_LENGTH 2147483648

class StringStreamTest : public ::testing::Test {
 protected:
  // Computes the capacity of the 'StringStream' instance using the Python list
  // resize routine that is identical to static function
  // '_ComputeStringStreamContCapacity()' inside module 'sstream.c'.
  void _ComputeStringStreamContCapacity(const size_t length,
                                        size_t* const capacity) {
    *capacity = (length >> 3) + (length < 9 ? 3 : 6);
    *capacity += length;
  }

  // De-allocate 'StringStream' instance from the free store.
  void TearDown() override { StringStreamDealloc(&sstream); }

 protected:
  StringStream sstream;
};

TEST_F(StringStreamTest, StringStreamDefAllocFunctionTest) {
  sstream = StringStreamDefAlloc();
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(sstream.length, SSTREAM_DEFAULT_SIZE);
  size_t capacity;
  _ComputeStringStreamContCapacity(sstream.length, &capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamTest, StringStreamAllocFunctionWithArbitraryLengthTest) {
  sstream = StringStreamAlloc(_ARBITRARY_TESTING_LENGTH);
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(sstream.length, _ARBITRARY_TESTING_LENGTH);
  size_t capacity;
  _ComputeStringStreamContCapacity(sstream.length, &capacity);

  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamTest, StringStreamStrAllocFunctionWithConstCharPointerTest) {
  const char* str = "StringStreamStrAllocFunctionWithConstCharPointerTest";
  sstream = StringStreamStrAlloc(str);
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(std::strcmp(sstream.data, str), 0);
  ASSERT_EQ(sstream.length, std::strlen(str));
  size_t capacity;
  _ComputeStringStreamContCapacity(sstream.length, &capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(
    StringStreamTest,
    StringStreamStrAllocFunctionWithConstCharPointerWithEmbededNullByteTest) {
  const char* str =
      "StringStream\0StrAllocFunctionWithConstCharPointerWithEmbededNullByteTes"
      "t";
  sstream = StringStreamStrAlloc(str);
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(std::strncmp(sstream.data, str, std::strlen(str)), 0);
  ASSERT_EQ(sstream.length, std::strlen(str));
  size_t capacity;
  _ComputeStringStreamContCapacity(sstream.length, &capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamTest, StringStreamReallocFunctionTestForReallocNotRequired) {
  sstream = StringStreamAlloc(_ARBITRARY_TESTING_LENGTH);
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(sstream.length, _ARBITRARY_TESTING_LENGTH);
  size_t capacity;
  _ComputeStringStreamContCapacity(sstream.length, &capacity);
  ASSERT_EQ(sstream.capacity, capacity);
  ASSERT_EQ(StringStreamRealloc(&sstream, _ARBITRARY_TESTING_LENGTH / 2),
            SSTREAM_REALLOC_NOT_REQUIRED);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamTest, StringStreamReallocFunctionTestForReallocSuccess) {
  sstream = StringStreamAlloc(_ARBITRARY_TESTING_LENGTH);
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(sstream.length, _ARBITRARY_TESTING_LENGTH);
  size_t capacity;
  _ComputeStringStreamContCapacity(sstream.length, &capacity);
  ASSERT_EQ(sstream.capacity, capacity);
  ASSERT_EQ(StringStreamRealloc(&sstream, _ARBITRARY_TESTING_LENGTH * 2),
            SSTREAM_REALLOC_SUCCESS);
  ASSERT_EQ(sstream.length, _ARBITRARY_TESTING_LENGTH);
  _ComputeStringStreamContCapacity(_ARBITRARY_TESTING_LENGTH * 2, &capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

#endif  // CJSON_TESTS_SSTREAM_TESTSSTREAM_HH_
