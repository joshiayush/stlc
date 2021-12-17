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

class SstreamTest : public ::testing::Test {
 protected:
  /**
   * @brief Deallocates @a stringstream instance from the free store.
   */
  void TearDown() override { stringstream_dealloc(&sstream); }

 protected:
  stringstream sstream;
};

TEST_F(SstreamTest, stringStreamDefAllocFunctionTest) {
  sstream = stringstream_def_alloc();
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(sstream.length, SSTREAM_DEFAULT_SIZE);
  uint32_t capacity = 1;
  while (capacity < sstream.length)
    capacity <<= 1;
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(SstreamTest, stringStreamAllocFunctionWithArbitraryLengthTest) {
  sstream = stringstream_alloc(2147483648);
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(sstream.length, 2147483648);
  uint32_t capacity = 1;
  while (capacity < sstream.length)
    capacity <<= 1;
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(SstreamTest, stringStreamStrAllocFunctionWithConstCharPointerTest) {
  const char* str = "stringStreamStrAllocFunctionWithConstCharPointerTest";
  sstream = stringstream_str_alloc(str);
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(std::strcmp(sstream.data, str), 0);
  ASSERT_EQ(sstream.length, std::strlen(str));
  uint32_t capacity = 1;
  while (capacity < sstream.length)
    capacity <<= 1;
  ASSERT_EQ(sstream.capacity, capacity);
}

#endif  // CJSON_TESTS_SSTREAM_TESTSSTREAM_HH_
