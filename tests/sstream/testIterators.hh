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

#include <cstring>

#include "data/sstream/iterators.h"
#include "data/sstream/sstream.h"

class StringStreamIteratorsTest : public ::testing::Test {
 protected:
  // De-allocate 'StringStream' instance from the free store.
  void TearDown() override { StringStreamDealloc(&sstream); }

 protected:
  StringStream sstream;
};

TEST_F(StringStreamIteratorsTest, StringStreamBeginTestWithNullByte) {
  sstream = StringStreamStrAlloc("\0");
  ASSERT_EQ(*StringStreamBegin(&sstream), (const char)'\0');
}

TEST_F(StringStreamIteratorsTest, StringStreamBeginTestWithCString) {
  const char* str = "StringStreamBeginTestWithCString";
  sstream = StringStreamStrAlloc(str);
  const char* begin = StringStreamBegin(&sstream);
  ASSERT_EQ(*begin, (const char)'S');
  ASSERT_EQ(std::strncmp(begin, str, sstream.length), 0);
}

TEST_F(StringStreamIteratorsTest, StringStreamEndTestWithNullByte) {
  sstream = StringStreamStrAlloc("\0");
  ASSERT_EQ(*StringStreamEnd(&sstream), (const char)'\0');
}

TEST_F(StringStreamIteratorsTest, StringStreamEndTestWithCString) {
  const char* str = "StringStreamBeginTestWithCString";
  sstream = StringStreamStrAlloc(str);
  const char* end = StringStreamEnd(&sstream);
  ASSERT_EQ(*end, (const char)'\0');
}

#endif  // CJSON_TESTS_SSTREAM_TESTITERATORS_HH_