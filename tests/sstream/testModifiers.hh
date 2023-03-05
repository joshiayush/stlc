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

#ifndef CJSON_TESTS_SSTREAM_TESTMODIFIERS_HH_
#define CJSON_TESTS_SSTREAM_TESTMODIFIERS_HH_

#include <gtest/gtest.h>

#include <cstring>

#include "sstream/sstream.h"
#include "utils.hh"

class StringStreamModifiersTest : public ::testing::Test {
 protected:
  void TearDown() override {
    if (sstream.data != nullptr)
      StringStreamDealloc(&sstream);
  }

 protected:
  StringStream sstream;
};

class StringStreamConcatTest : public StringStreamModifiersTest {};

TEST_F(StringStreamConcatTest, StringStreamConcatTest) {
  sstream = StringStreamAlloc();
  StringStreamConcat(&sstream, "");
  EXPECT_EQ(std::strlen(sstream.data), 0);
  EXPECT_EQ(sstream.length, 0);
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      SSTREAM_DEFAULT_SIZE, capacity);
  ASSERT_EQ(sstream.capacity, capacity);

  StringStreamConcat(&sstream, "Hello, %s!", "world");
  EXPECT_STREQ(sstream.data, "Hello, world!");
  EXPECT_EQ(sstream.length, std::strlen("Hello, world!"));
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  ASSERT_EQ(sstream.capacity, capacity);

  // This string will overflow the buffer which will require the
  // "StringStreamConcat" function to reallocate the "StringStream" buffer in
  // order to copy the large string.
  StringStreamConcat(&sstream,
                     "This is a very long string that will "
                     "definitely exceed the size of the buffer "
                     "and cause a reallocation to occur. "
                     "This is a test string, so don't worry.");
  EXPECT_EQ(sstream.length,
            std::strlen("Hello, world!") +
                std::strlen("This is a very long string that will "
                            "definitely exceed the size of the buffer "
                            "and cause a reallocation to occur. "
                            "This is a test string, so don't worry."));
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  EXPECT_EQ(sstream.capacity, capacity);
}

class StringStreamReadTest : public StringStreamModifiersTest {};

TEST_F(StringStreamReadTest, StringStreamReadTest) {
  sstream = StringStreamAlloc();

  const char* data = "This is a test string.";
  size_t length = std::strlen(data);

  StringStreamRead(&sstream, "", 0);
  EXPECT_EQ(std::strlen(sstream.data), 0);
  EXPECT_EQ(sstream.length, 0);

  StringStreamRead(&sstream, data, length);
  EXPECT_STREQ(sstream.data, data);
  EXPECT_EQ(sstream.length, length);

  StringStreamRead(&sstream, data, SSTREAM_DEFAULT_SIZE + 10);
  EXPECT_EQ(sstream.length, length + SSTREAM_DEFAULT_SIZE + 10);
}

class StringStreamRetreatTest : public StringStreamModifiersTest {};

TEST_F(StringStreamRetreatTest, StringStreamRetreatTest) {
  sstream = StringStreamAlloc();
  const char* data = "This is a test string.";
  size_t length = std::strlen(data);

  StringStreamRetreat(&sstream, 5);
  EXPECT_EQ(std::strlen(sstream.data), 0);
  EXPECT_EQ(sstream.length, 0);

  StringStreamRead(&sstream, data, length);
  StringStreamRetreat(&sstream, 5);
  EXPECT_EQ(sstream.length, length - 5);
  EXPECT_EQ(sstream.data[sstream.length], '\0');

  StringStreamRetreat(&sstream, length + 5);
  EXPECT_EQ(sstream.length, 0);
  EXPECT_EQ(sstream.data[sstream.length], '\0');
}

#endif  // CJSON_TESTS_SSTREAM_TESTMODIFIERS_HH_
