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

#include <string>

#include "bool.h"
#include "data/sstream/sstream.h"

TEST(ChrCStrLiteralTest, TestWithNonControlCharacters) {
  const char* str = "//foo//bar//buzz";
  const size_t strlen_ = sizeof(str);

  char* buffer =
      reinterpret_cast<char*>(std::malloc(sizeof(strlen_) * sizeof(char)));
  for (size_t i = 0; i < strlen_; ++i)
    *(buffer + i) = '\0';
  for (size_t i = 0; i < strlen_; ++i)
    ChrCStrLiteral(str[i], buffer);

  EXPECT_STREQ(buffer, "//foo//bar//buzz");

  std::free(buffer);
}

TEST(PrintSstreamTest, TestPrintSstreamWhenPrintedAString) {
  StringStream sstream = StringStreamStrAlloc("foo//bar//buzz");

  testing::internal::CaptureStdout();
  PrintSstream(&sstream, FALSE);
  std::string stdout_output = testing::internal::GetCapturedStdout();

  EXPECT_STREQ(stdout_output.c_str(), "foo//bar//buzz");

  StringStreamDealloc(&sstream);
}

TEST(PrintSstreamTest, TestPrintSstreamWithAStringContainingEscapeSequences) {
  StringStream sstream = StringStreamStrAlloc("\foo\bar");

  testing::internal::CaptureStdout();
  PrintSstream(&sstream, TRUE);
  std::string stdout_output = testing::internal::GetCapturedStdout();

  EXPECT_STREQ(stdout_output.c_str(), "\\foo\\bar");

  StringStreamDealloc(&sstream);
}

#endif  // CJSON_TESTS_SSTREAM_TESTPRINTERS_HH_
