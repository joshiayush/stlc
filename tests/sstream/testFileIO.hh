// Copyright 2021, The cjson authors Inc. All rights reserved.
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
//     * Neither the name of The cjson authors Inc. nor the names of its
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

#include "data/sstream/sstream.h"

class StringStreamFileIOTest : public ::testing::Test {
 protected:
  void TearDown() override {
    if (sstream.data != NULL)
      StringStreamDealloc(&sstream);
  }

 protected:
  StringStream sstream;
};

// clang-format off
const char* kFileCopyRightText =
    "// Copyright 2021, The cjson authors Inc. All rights reserved.\n"
    "//\n"
    "// Redistribution and use in source and binary forms, with or without\n"
    "// modification, are permitted provided that the following conditions are\n"
    "// met:\n"
    "//\n"
    "//     * Redistributions of source code must retain the above copyright\n"
    "// notice, this list of conditions and the following disclaimer.\n"
    "//     * Redistributions in binary form must reproduce the above\n"
    "// copyright notice, this list of conditions and the following disclaimer\n"
    "// in the documentation and/or other materials provided with the\n"
    "// distribution.\n"
    "//     * Neither the name of The cjson authors Inc. nor the names of its\n"
    "// contributors may be used to endorse or promote products derived from\n"
    "// this software without specific prior written permission.\n"
    "//\n"
    "// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS\n"
    "// \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT\n"
    "// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR\n"
    "// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT\n"
    "// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,\n"
    "// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT\n"
    "// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,\n"
    "// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY\n"
    "// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n"
    "// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE\n"
    "// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n";
// clang-format on

TEST_F(StringStreamFileIOTest,
       StringStreamFileIOTestWithCFileCopyRightMessage) {
  std::FILE* file = std::fopen(__FILE__, "r");
  StringStreamReadFile(&sstream, file, 0);
  std::fclose(file);
  EXPECT_EQ(std::strncmp(sstream.data, kFileCopyRightText,
                         std::strlen(kFileCopyRightText)),
            0)
      << "Actual data\n"
      << sstream.data << "\nExpected data\n"
      << kFileCopyRightText;
}

#endif  // CJSON_TESTS_SSTREAM_TESTFILEIO_HH_
