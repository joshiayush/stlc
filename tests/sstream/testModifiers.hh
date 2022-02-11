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

#ifndef CJSON_TESTS_SSTREAM_TESTMODIFIERS_HH_
#define CJSON_TESTS_SSTREAM_TESTMODIFIERS_HH_

#include <gtest/gtest.h>

#include <cstring>

#include "data/sstream/sstream.h"
#include "utils.hh"

class StringStreamModifiersTest : public ::testing::Test {
 protected:
  void TearDown() override { StringStreamDealloc(&sstream); }

 protected:
  StringStream sstream;
};

TEST_F(StringStreamModifiersTest,
       StringStreamConcatTestWithAEmptyStringStreamInstance) {
  const char* teststr_ = "Mohika is really sweet";

  sstream = StringStreamAlloc();
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(sstream.length, 0);
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      SSTREAM_DEFAULT_SIZE, capacity);
  ASSERT_EQ(sstream.capacity, capacity);

  StringStreamConcat(&sstream, "%s %s", teststr_, "and charming :).");
  const char* data_ = "Mohika is really sweet and charming :).";

  ASSERT_EQ(std::strcmp(sstream.data, data_), 0)
      << "Expected equality of these values:\n"
      << "  sstream.data\n"
      << "    Which is: " << sstream.data << "\n"
      << "  data_\n"
      << "    Which is: " << data_;
  ASSERT_EQ(sstream.length, 39);
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamModifiersTest,
       StringStreamConcatTestWithAStrAllocatedStringStreamInstance) {
  const char* teststr_ = "Mohika";

  sstream = StringStreamStrAlloc(teststr_);
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(sstream.length, std::strlen(teststr_));
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  ASSERT_EQ(sstream.capacity, capacity);

  StringStreamConcat(&sstream, " %s %s %d%s", "is really sweet :)",
                     "and talented with a charisma of", 100, "%.");
  const char* data_ =
      "Mohika is really sweet :) and talented with a charisma of 100%.";

  ASSERT_EQ(std::strcmp(sstream.data, data_), 0)
      << "Expected equality of these values:\n"
      << "  sstream.data\n"
      << "    Which is: " << sstream.data << "\n"
      << "  data_\n"
      << "    Which is: " << data_;
  ASSERT_EQ(sstream.length, 63);
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamModifiersTest,
       StringStreamReadTestWithADefaultAllocatedStringStreamInstance) {
  sstream = StringStreamAlloc();
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(sstream.length, 0);
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      SSTREAM_DEFAULT_SIZE, capacity);
  ASSERT_EQ(sstream.capacity, capacity);

  const char* readstr =
      "Mohika is really sweet :) and talented with a charisma of 100%.";
  StringStreamRead(&sstream, readstr, std::strlen(readstr));

  ASSERT_EQ(std::strcmp(sstream.data, readstr), 0)
      << "Expected equality of these values:\n"
      << "  sstream.data\n"
      << "    Which is: " << sstream.data << "\n"
      << "  data_\n"
      << "    Which is: " << readstr;
  ASSERT_EQ(sstream.length, std::strlen(readstr));
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamModifiersTest,
       StringStreamReadTestWithAAllocatedStringStreamInstance) {
  const char* readstr =
      "Mohika is really sweet :) and talented with a charisma of 100%.";

  sstream = StringStreamNAlloc(std::strlen(readstr));
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(sstream.length, 0);
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      std::strlen(readstr), capacity);
  ASSERT_EQ(sstream.capacity, capacity);

  StringStreamRead(&sstream, readstr, std::strlen(readstr));

  ASSERT_EQ(std::strcmp(sstream.data, readstr), 0)
      << "Expected equality of these values:\n"
      << "  sstream.data\n"
      << "    Which is: " << sstream.data << "\n"
      << "  readstr\n"
      << "    Which is: " << readstr;
  ASSERT_EQ(sstream.length, std::strlen(readstr));
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      std::strlen(readstr), capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(StringStreamModifiersTest,
       StringStreamReadTestWithAStrAllocatedStringStreamInstance) {
  const char* teststr_ = "Mohika";

  sstream = StringStreamStrAlloc(teststr_);
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(sstream.length, std::strlen(teststr_));
  size_t capacity;
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  ASSERT_EQ(sstream.capacity, capacity);

  const char* readstr =
      ", the happiest people don't have the best of everything, they just make "
      "the best of everything.";
  StringStreamRead(&sstream, readstr, std::strlen(readstr));
  const char* data_ =
      "Mohika, the happiest people don't have the best of everything, they "
      "just make the best of everything.";

  ASSERT_EQ(std::strcmp(sstream.data, data_), 0)
      << "Expected equality of these values:\n"
      << "  sstream.data\n"
      << "    Which is: " << sstream.data << "\n"
      << "  data_\n"
      << "    Which is: " << data_;
  ASSERT_EQ(sstream.length, std::strlen(data_));
  cjson::testing::sstream::utils::ComputeStringStreamBufferCapacity(
      sstream.length, capacity);
  ASSERT_EQ(sstream.capacity, capacity);
}

#endif  // CJSON_TESTS_SSTREAM_TESTMODIFIERS_HH_
