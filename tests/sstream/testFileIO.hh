// Copyright 2021, The cjson authors. All rights reserved.
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

#ifndef CJSON_TESTS_SSTREAM_TESTFILEIO_HH_
#define CJSON_TESTS_SSTREAM_TESTFILEIO_HH_

#include <gtest/gtest.h>

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "sstream/sstream.h"

TEST(StringStreamReadFile, StringStreamReadFileTest) {
  StringStream sstream = StringStreamAlloc();
  FILE* file = std::fopen("test.txt", "w");
  ASSERT_NE(file, nullptr);
  const char* content = "This is a test file.\n";
  std::fputs(content, file);
  std::fclose(file);

  file = std::fopen("test.txt", "r");
  ASSERT_NE(file, nullptr);

  StringStreamReadFile(&sstream, file, std::strlen(content));
  std::fclose(file);
  EXPECT_EQ(sstream.length, std::strlen(content));
  EXPECT_STREQ(sstream.data, content);

  std::remove("test.txt");
  StringStreamDealloc(&sstream);
}

TEST(StringStreamWriteFile, StringStreamWriteFileTest) {
  StringStream sstream = StringStreamAlloc();
  StringStreamConcat(&sstream, "This is a test string.");
  FILE* file = std::fopen("test.txt", "w");
  ASSERT_NE(file, nullptr);
  StringStreamWriteFile(&sstream, file, 5, 10);
  std::fclose(file);

  file = std::fopen("test.txt", "r");
  ASSERT_NE(file, nullptr);
  char buffer[50];
  std::fgets(buffer, 50, file);
  std::fclose(file);
  EXPECT_STREQ(buffer, "is a ");

  std::remove("test.txt");
  StringStreamDealloc(&sstream);
}

#endif  // CJSON_TESTS_SSTREAM_TESTFILEIO_HH_
