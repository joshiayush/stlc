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

#ifndef CJSON_TESTS_SSTREAM_TESTSSTREAM_HH_
#define CJSON_TESTS_SSTREAM_TESTSSTREAM_HH_

#include <gtest/gtest.h>
#include <stdio.h>

#include <cstring>

#include "data/sstream/sstream.h"
#include "utils.hh"

#define _ARBITRARY_SSTREAM_TESTING_LENGTH 2147483648

class StringStreamTest : public ::testing::Test {
 protected:
  void TearDown() override {
    if (sstream.data != nullptr)
      StringStreamDealloc(&sstream);
  }

 protected:
  StringStream sstream;
};

TEST_F(StringStreamTest, WhenAllocatedWithDefAlloc) {
  sstream = StringStreamAlloc();
  ASSERT_NE(sstream.data, nullptr);
  ASSERT_EQ(*sstream.data, '\0');
  ASSERT_EQ(sstream.length, 0);
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      SSTREAM_DEFAULT_SIZE, capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamTest, WhenAllocatedWithAllocWithArbitrarySstreamLength) {
  sstream = StringStreamNAlloc(_ARBITRARY_SSTREAM_TESTING_LENGTH);
  ASSERT_NE(sstream.data, nullptr);
  ASSERT_EQ(*sstream.data, '\0');
  ASSERT_EQ(sstream.length, 0);
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      _ARBITRARY_SSTREAM_TESTING_LENGTH, capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamTest, WhenAllocatedWithAllocWithConstCharPointer) {
  const char* teststr =
      "Mohika says \"I'm that exam question that everyone left without "
      "understanding :(.\"";
  sstream = StringStreamStrAlloc(teststr);
  ASSERT_NE(sstream.data, nullptr);
  ASSERT_STREQ(sstream.data, teststr);
  ASSERT_EQ(sstream.length, std::strlen(teststr));
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamTest,
       WhenAllocatedWithAllocWithConstCharPointerAndEmbededNullBytes) {
  const char* teststr =
      "Mohika\0says\0\"I'm\0that\0exam\0question\0that\0everyone\0left"
      "\0without\0understanding\0:(.\"";
  sstream = StringStreamStrAlloc(teststr);
  ASSERT_NE(sstream.data, nullptr);
  ASSERT_STREQ(sstream.data, teststr);
  ASSERT_EQ(sstream.length, std::strlen(teststr));
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamTest,
       WhenAllocatedWithNAllocWithConstCharPointerAndEmbededNullBytes) {
  const char* teststr =
      "Mohika\0says\0\"I'm\0that\0exam\0question\0that\0everyone\0left"
      "\0without\0understanding\0:(\";"
      "\0\0I\0did\0not\0get\0a\0chance\0to\0even\0attempt\0that\0exam\0:(.";
  const size_t strlen_ = 134;
  sstream = StringStreamStrNAlloc(teststr, strlen_);
  ASSERT_NE(sstream.data, nullptr);
  ASSERT_EQ(std::strncmp(sstream.data, teststr, strlen_), 0);
  ASSERT_EQ(sstream.length, strlen_);
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamTest, WhenReallocatedWithHalfOfTheActaulCapacity) {
  sstream = StringStreamNAlloc(_ARBITRARY_SSTREAM_TESTING_LENGTH);
  ASSERT_NE(sstream.data, nullptr);
  ASSERT_EQ(sstream.length, 0);
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      _ARBITRARY_SSTREAM_TESTING_LENGTH, capacity);
  ASSERT_EQ(sstream.capacity, capacity);
  ASSERT_EQ(
      StringStreamRealloc(&sstream, _ARBITRARY_SSTREAM_TESTING_LENGTH / 2),
      SSTREAM_REALLOC_NOT_REQUIRED);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamTest, WhenReallocatedWithTwiceOfTheActaulCapacity) {
  sstream = StringStreamNAlloc(_ARBITRARY_SSTREAM_TESTING_LENGTH);
  ASSERT_NE(sstream.data, nullptr);
  ASSERT_EQ(sstream.length, 0);
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      _ARBITRARY_SSTREAM_TESTING_LENGTH, capacity);
  ASSERT_EQ(sstream.capacity, capacity);
  ASSERT_EQ(
      StringStreamRealloc(&sstream, _ARBITRARY_SSTREAM_TESTING_LENGTH * 2),
      SSTREAM_REALLOC_SUCCESS);
  ASSERT_EQ(sstream.length, 0);
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      _ARBITRARY_SSTREAM_TESTING_LENGTH * 2, capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamTest, WhenDeallocatedADefAllocatedSstream) {
  sstream = StringStreamAlloc();
  ASSERT_NE(sstream.data, nullptr);
  ASSERT_EQ(sstream.length, 0);
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      SSTREAM_DEFAULT_SIZE, capacity);
  ASSERT_EQ(sstream.capacity, capacity);

  StringStreamDealloc(&sstream);
  ASSERT_EQ(sstream.data, nullptr);
  ASSERT_EQ(sstream.length, 0);
  ASSERT_EQ(sstream.capacity, 0);
}

TEST_F(StringStreamTest, WhenDeallocatedANonDefaultAllocatedSstream) {
  sstream = StringStreamNAlloc(_ARBITRARY_SSTREAM_TESTING_LENGTH);
  ASSERT_NE(sstream.data, nullptr);
  ASSERT_EQ(sstream.length, 0);
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      _ARBITRARY_SSTREAM_TESTING_LENGTH, capacity);
  ASSERT_EQ(sstream.capacity, capacity);

  StringStreamDealloc(&sstream);
  ASSERT_EQ(sstream.data, nullptr);
  ASSERT_EQ(sstream.length, 0);
  ASSERT_EQ(sstream.capacity, 0);
}

TEST_F(StringStreamTest, WhenDeallocatedAStrNAllocatedSstream) {
  const char* teststr =
      "Mohika says \"I'm that exam question that everyone left without "
      "understanding :(\"; I did not get a chance to even attempt that exam "
      ":(.";
  const size_t strlen_ = 134;
  sstream = StringStreamStrNAlloc(teststr, strlen_);
  ASSERT_NE(sstream.data, nullptr);
  ASSERT_STREQ(sstream.data, teststr);
  ASSERT_EQ(sstream.length, strlen_);
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  ASSERT_EQ(sstream.capacity, capacity);

  StringStreamDealloc(&sstream);
  ASSERT_EQ(sstream.data, nullptr);
  ASSERT_EQ(sstream.length, 0);
  ASSERT_EQ(sstream.capacity, 0);
}

TEST_F(StringStreamTest, WhenDeallocatedAStrAllocWithEmbededNullBytes) {
  const char* teststr =
      "Mohika\0says\0\"I'm\0that\0exam\0question\0that\0everyone\0left"
      "\0without\0understanding\0:(.\"";
  sstream = StringStreamStrAlloc(teststr);
  ASSERT_NE(sstream.data, nullptr);
  ASSERT_STREQ(sstream.data, teststr);
  ASSERT_EQ(sstream.length, std::strlen(teststr));
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  ASSERT_EQ(sstream.capacity, capacity);

  StringStreamDealloc(&sstream);
  ASSERT_EQ(sstream.data, nullptr);
  ASSERT_EQ(sstream.length, 0);
  ASSERT_EQ(sstream.capacity, 0);
}

TEST_F(StringStreamTest,
       WhenDeallocatedANAllocatedSstreamWithEmbededNullBytes) {
  const char* teststr =
      "Mohika\0says\0\"I'm\0that\0exam\0question\0that\0everyone\0left"
      "\0without\0understanding\0:(\";"
      "\0\0I\0did\0not\0get\0a\0chance\0to\0even\0attempt\0that\0exam\0:(.";
  const size_t strlen_ = 134;
  sstream = StringStreamStrNAlloc(teststr, strlen_);
  ASSERT_NE(sstream.data, nullptr);
  ASSERT_STREQ(sstream.data, teststr);
  ASSERT_EQ(sstream.length, strlen_);
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  ASSERT_EQ(sstream.capacity, capacity);

  StringStreamDealloc(&sstream);
  ASSERT_EQ(sstream.data, nullptr);
  ASSERT_EQ(sstream.length, 0);
  ASSERT_EQ(sstream.capacity, 0);
}

#endif  // CJSON_TESTS_SSTREAM_TESTSSTREAM_HH_
