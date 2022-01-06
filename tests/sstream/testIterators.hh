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

#ifndef CJSON_TESTS_SSTREAM_TESTITERATORS_HH_
#define CJSON_TESTS_SSTREAM_TESTITERATORS_HH_

#include <gtest/gtest.h>
#include <stdio.h>

#include <cstring>

#include "data/sstream/sstream.h"

class StringStreamIteratorsTest : public ::testing::Test {
 protected:
  void TearDown() override {
    if (sstream.data != NULL)
      StringStreamDealloc(&sstream);
  }

 protected:
  StringStream sstream;
};

TEST_F(StringStreamIteratorsTest, StringStreamBeginTestWithNullByte) {
  sstream = StringStreamStrAlloc("\0");
  EXPECT_EQ(*StringStreamBegin(&sstream), (const char)'\0');
}

TEST_F(StringStreamIteratorsTest, StringStreamBeginTestWithCString) {
  const char* str = "Mohika, I miss your smile.";
  sstream = StringStreamStrAlloc(str);
  const char* begin = StringStreamBegin(&sstream);
  EXPECT_EQ(*begin, (const char)'M');
  EXPECT_EQ(std::strncmp(begin, str, sstream.length), 0);
}

TEST_F(StringStreamIteratorsTest, StringStreamEndTestWithNullByte) {
  sstream = StringStreamStrAlloc("\0");
  EXPECT_EQ(*StringStreamEnd(&sstream), (const char)'\0');
}

TEST_F(StringStreamIteratorsTest, StringStreamEndTestWithCString) {
  const char* str = "Mohika, I miss your smile.";
  sstream = StringStreamStrAlloc(str);
  EXPECT_EQ(*StringStreamEnd(&sstream), (const char)'\0');
}

TEST_F(StringStreamIteratorsTest,
       StringStreamEndTestWithCStringWithEmbededNullByte) {
  const char* str = "Mohika,\0I miss your smile.";
  sstream = StringStreamStrAlloc(str);
  EXPECT_EQ(*StringStreamEnd(&sstream), (const char)'\0');
  EXPECT_EQ(*(StringStreamEnd(&sstream) - 1), (const char)',');
}

TEST_F(
    StringStreamIteratorsTest,
    StringStreamEndTestWithCStringWithEmbededNullByteWhenAllocatedWithStringStreamStrNAlloc) {
  const char* str = "Mohika,\0I miss your smile.";
  const size_t strlen_ = 26;
  sstream = StringStreamStrNAlloc(str, strlen_);
  EXPECT_EQ(*StringStreamEnd(&sstream), (const char)'\0');
  EXPECT_EQ(*(StringStreamEnd(&sstream) - 1), (const char)'.');
}

#endif  // CJSON_TESTS_SSTREAM_TESTITERATORS_HH_
