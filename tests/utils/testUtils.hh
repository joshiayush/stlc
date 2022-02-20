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

#include <gtest/gtest.h>

#include <cstring>

#include "utils/utils.h"

TEST(UtilitySplitStrFunction, TestUtilitySplitStr) {
  char** comps = SplitStr("/foo/bar/foo/buzz/", "/");
  const char* exp_splited_comps[] = {"foo", "bar", "foo", "buzz"};
  for (size_t i = 0; i < 4; ++i) {
    EXPECT_EQ(std::strncmp(comps[i], exp_splited_comps[i],
                           std::strlen(exp_splited_comps[i])),
              0)
        << "comps[" << i << "]: " << comps[i] << "\nexp_splited_comps[" << i
        << "]: " << exp_splited_comps[i];
    std::free(*(comps + i));
  }
  std::free(comps);
}

TEST(UtilsFunctionTest, TestIsAbsPathFunction) {
  EXPECT_TRUE(IsAbsPath("/"));
  EXPECT_TRUE(IsAbsPath(__FILE__));
}

class UtilitySplitFunctionTest : public ::testing::Test {
 protected:
  char head[100];
  char tail[100];
};

TEST_F(UtilitySplitFunctionTest, TestWhenOnlyRootNodeIsGiven) {
  const char* path = "/";
  Split(head, tail, path);

  EXPECT_EQ(std::strcmp(head, path), 0);
  EXPECT_EQ(*tail, (const char)'\0');
}

TEST_F(UtilitySplitFunctionTest, TestWhenOnlyFileIsGiven) {
  const char* path = "CMakeLists.txt";
  Split(head, tail, path);

  EXPECT_EQ(*head, (const char)'\0');
  EXPECT_EQ(std::strcmp(tail, path), 0);
}

TEST_F(UtilitySplitFunctionTest, TestWhenCurrentFilePathIsUsed) {
  const char* path = "/C/cjson/tests/utils/testUtils.hh";
  Split(head, tail, path);

  EXPECT_EQ(std::strcmp(head, "/C/cjson/tests/utils"), 0);
  EXPECT_EQ(std::strcmp(tail, "testUtils.hh"), 0);
}

class UtilityProtectedGetCurrentWorkingDirTest : public ::testing::Test {
 protected:
  char buffer[100];
};

TEST_F(UtilityProtectedGetCurrentWorkingDirTest,
       TestWhenOnlyRootNodeIsGivenAsAbspath) {
  const char* dir = "/";

  char* cwd_ = _GetCurrentWorkingDir(dir, buffer, sizeof(buffer));

  EXPECT_EQ(std::strcmp(buffer, dir), 0);
  EXPECT_EQ(std::strcmp(buffer, cwd_), 0);
}

TEST_F(UtilityProtectedGetCurrentWorkingDirTest,
       TestWhenOnlyFileIsUsedAsAbspath) {
  const char* dir = "CMakeLists.txt";
  const char* cwd = "";

  char* cwd_ = _GetCurrentWorkingDir(dir, buffer, sizeof(buffer));

  EXPECT_EQ(std::strcmp(buffer, cwd), 0);
  EXPECT_EQ(std::strcmp(buffer, cwd_), 0);
}

TEST_F(UtilityProtectedGetCurrentWorkingDirTest,
       TestWhenCurrentFilePathIsUsedAsAbspath) {
  const char* dir = "/C/cjson/tests/utils/testUtils.hh";
  const char* cwd = "/C/cjson/tests/utils";

  char* cwd_ = _GetCurrentWorkingDir(dir, buffer, sizeof(buffer));

  EXPECT_EQ(std::strcmp(buffer, cwd), 0);
  EXPECT_EQ(std::strcmp(buffer, cwd_), 0);
}

TEST_F(UtilityProtectedGetCurrentWorkingDirTest, TestWhenFILEMacroIsUsed) {
  char head[100], tail[100];
  Split(head, tail, __FILE__);

  char* cwd_ = GetCurrentWorkingDir(buffer, sizeof(buffer));

  EXPECT_EQ(std::strcmp(buffer, head), 0);
  EXPECT_EQ(std::strcmp(cwd_, buffer), 0);
}

class UtilityJoinFunctionTest : public ::testing::Test {
 protected:
  char buffer[100];
};

TEST_F(UtilityJoinFunctionTest, TestJoinFunctionWithMultipleAbsPaths) {
  const char* joined_path = Join(sizeof(buffer), buffer, 2, "/C", "/cjson");
  EXPECT_EQ(std::strcmp(buffer, "/cjson"), 0) << "buffer: " << buffer << '\n';
}

TEST_F(UtilityJoinFunctionTest, TestJoinFunctionWhenOneFilePathIsUsed) {
  const char* joined_path =
      Join(sizeof(buffer), buffer, 4, "C", "cjson", "/cjson", "CMakeLists.txt");

  EXPECT_EQ(std::strcmp(buffer, "/cjson/CMakeLists.txt"), 0)
      << "buffer: " << buffer << '\n';
}
