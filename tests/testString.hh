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

#include <gtest/gtest.h>

#include <cstring>

#include "stlc-string.h"

TEST(SplitStrFunctionTest, WhenSingleCharacterIsUsedAsSeparator) {
  char** comps = SplitStr("/foo/bar/foo/buzz/", "/");
  const char* exp_splited_comps[] = {"foo", "bar", "foo", "buzz"};
  for (size_t i = 0; i < 4; ++i) {
    EXPECT_EQ(std::strncmp(comps[i], exp_splited_comps[i],
                           std::strlen(exp_splited_comps[i])),
              0)
        << "comps[" << i << "]: " << comps[i] << "\nexp_splited_comps[" << i
        << "]: " << exp_splited_comps[i] << '\n';
    std::free(*(comps + i));
  }
  std::free(comps);
}

TEST(SplitStrFunctionTest, WhenMultipleCharactersAreUsedAsSeparator) {
  char** comps = SplitStr("//foo//bar//foo//buzz//", "//");
  const char* exp_splited_comps[] = {"foo", "bar", "foo", "buzz"};
  for (size_t i = 0; i < 4; ++i) {
    EXPECT_EQ(std::strncmp(comps[i], exp_splited_comps[i],
                           std::strlen(exp_splited_comps[i])),
              0)
        << "comps[" << i << "]: " << comps[i] << "\nexp_splited_comps[" << i
        << "]: " << exp_splited_comps[i] << '\n';
    std::free(*(comps + i));
  }
  std::free(comps);
}

TEST(JoinStrFunctionTest, WhenSingleCharacterIsUsedAsSeparator) {
  char joined_str[100];
  const char* const comps[] = {"foo", "bar", "foo", "buzz"};
  JoinStr(joined_str, "/", comps, 4);
  const char* exp_joined_str = "foo/bar/foo/buzz";
  EXPECT_EQ(
      std::strncmp(joined_str, exp_joined_str, std::strlen(exp_joined_str)), 0)
      << "joined_str: " << joined_str << "\nexp_joined_str: " << exp_joined_str
      << '\n';
}

TEST(JoinStrFunctionTest, WhenMultipleCharactersAreUsedAsSeparator) {
  char joined_str[100];
  const char* const comps[] = {"foo", "bar", "foo", "buzz"};
  JoinStr(joined_str, "//", comps, 4);
  const char* exp_joined_str = "foo//bar//foo//buzz";
  EXPECT_EQ(
      std::strncmp(joined_str, exp_joined_str, std::strlen(exp_joined_str)), 0)
      << "joined_str: " << joined_str << "\nexp_joined_str: " << exp_joined_str
      << '\n';
}
