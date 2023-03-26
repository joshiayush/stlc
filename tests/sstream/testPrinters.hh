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

#ifndef CJSON_TESTS_SSTREAM_TESTPRINTERS_HH_
#define CJSON_TESTS_SSTREAM_TESTPRINTERS_HH_

#include <gtest/gtest.h>

#include <cstring>
#include <string>

#include "bool.h"
#include "sstream/sstream.h"

#define _MAX_BUFFER_CAPACITY 1024

class ChrCStrLiteralTest : public ::testing::Test {
 protected:
  void SetUp() override { std::memset(buffer, 0, sizeof(buffer)); }

 protected:
  char buffer[_MAX_BUFFER_CAPACITY];
};

TEST_F(ChrCStrLiteralTest, TestWithControlCharacters) {
  memset(buffer, 0, sizeof(buffer));
  ChrCStrLiteral('\'', buffer);
  ASSERT_STREQ(buffer, "\\'");

  memset(buffer, 0, sizeof(buffer));
  ChrCStrLiteral('\r', buffer);
  ASSERT_STREQ(buffer, "\\r");

  memset(buffer, 0, sizeof(buffer));
  ChrCStrLiteral('\x1b', buffer);
  ASSERT_STREQ(buffer, "\\0x1b");

  memset(buffer, 0, sizeof(buffer));
  ChrCStrLiteral('\0', buffer);
  ASSERT_STREQ(buffer, "\\0x00");
}

TEST_F(ChrCStrLiteralTest, TestWithNonControlCharacters) {
  ChrCStrLiteral('A', buffer);
  ASSERT_STREQ(buffer, "A");
}

class ReprSstreamTest : public ::testing::Test {
 protected:
  void TearDown() override {
    if (sstream.data != nullptr) StringStreamFree(&sstream);
  }

 protected:
  StringStream sstream;
};

TEST_F(ReprSstreamTest, ReprSstreamTest) {
  StringStreamStrInit(&sstream, "", -1);
  ReprSstream(&sstream);
  ASSERT_STREQ(sstream.data, "");
  StringStreamFree(&sstream);

  StringStreamStrInit(&sstream, "This is a\ttest string.\n", -1);
  ReprSstream(&sstream);
  ASSERT_STREQ(sstream.data, "This is a\\ttest string.\\n");
  StringStreamFree(&sstream);

  StringStreamStrInit(&sstream, "\t\n\r", -1);
  ReprSstream(&sstream);
  ASSERT_STREQ(sstream.data, "\\t\\n\\r");
  StringStreamFree(&sstream);
}

class PrintSstreamTest : public ::testing::Test {
 protected:
  void TearDown() override {
    if (sstream.data != nullptr) StringStreamFree(&sstream);
  }

 protected:
  StringStream sstream;
};

TEST_F(PrintSstreamTest, PrintSstreamTestWhenNotEscaped) {
  StringStreamStrInit(&sstream, "Hello, World!\n", -1);
  testing::internal::CaptureStdout();
  PrintSstream(&sstream, false);
  std::string output = testing::internal::GetCapturedStdout();
  ASSERT_STREQ(output.c_str(), sstream.data);
}

TEST_F(PrintSstreamTest, PrintSstreamTestWhenEscaped) {
  StringStreamStrInit(&sstream, "", -1);
  testing::internal::CaptureStdout();
  PrintSstream(&sstream, true);
  std::string output = testing::internal::GetCapturedStdout();
  ASSERT_STREQ(output.c_str(), sstream.data);
  StringStreamFree(&sstream);

  StringStreamStrInit(&sstream, "This is a\ttest string.\n", -1);
  testing::internal::CaptureStdout();
  PrintSstream(&sstream, true);
  output = testing::internal::GetCapturedStdout();
  ASSERT_STREQ(output.c_str(), "This is a\\ttest string.\\n");
  StringStreamFree(&sstream);

  StringStreamStrInit(&sstream, "\t\n\r", -1);
  testing::internal::CaptureStdout();
  PrintSstream(&sstream, true);
  output = testing::internal::GetCapturedStdout();
  ASSERT_STREQ(output.c_str(), "\\t\\n\\r");
  StringStreamFree(&sstream);
}

#endif  // CJSON_TESTS_SSTREAM_TESTPRINTERS_HH_
