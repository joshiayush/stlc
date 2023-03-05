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

#ifndef CJSON_TESTS_SSTREAM_TESTACCESSORS_HH_
#define CJSON_TESTS_SSTREAM_TESTACCESSORS_HH_

#include <gtest/gtest.h>

#include "sstream/sstream.h"

class GetStringStreamAvailableSpaceTest : public ::testing::Test {
 protected:
  void TearDown() override {
    if (sstream.data != NULL)
      StringStreamDealloc(&sstream);
  }

 protected:
  StringStream sstream;
};

TEST_F(GetStringStreamAvailableSpaceTest, GetStringStreamAvailableSpaceTest) {
  sstream = StringStreamAlloc();
  ASSERT_EQ(_GET_STRING_STREAM_AVAILABLE_SPACE(sstream),
            sstream.capacity - sstream.length - 1);

  const size_t strlen_ = 2147483648;
  sstream = StringStreamNAlloc(strlen_);
  ASSERT_EQ(_GET_STRING_STREAM_AVAILABLE_SPACE(sstream),
            sstream.capacity - sstream.length - 1);
}

#endif  // CJSON_TESTS_SSTREAM_TESTACCESSORS_HH_
