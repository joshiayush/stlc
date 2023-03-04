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

#include "stlc-fs.h"

TEST(IsAbsPathTest, TestForBothWindowsAndNixStylePaths) {
#ifdef CJSON_OS_WINDOWS
  EXPECT_TRUE(IsAbsPath("C:\\"));
  EXPECT_TRUE(IsAbsPath("C:\\foo\\bar\\buzz"));
#else
  EXPECT_TRUE(IsAbsPath("/"));
  EXPECT_TRUE(IsAbsPath("/foo/bar/buzz"));
#endif
}

class SplitFunctionTest : public ::testing::Test {
 protected:
  char head[100];
  char tail[100];
};

TEST_F(SplitFunctionTest, WhenOnlyRootNodeIsGiven) {
  const char* path = "/";
  Split(head, tail, path);

  EXPECT_EQ(std::strcmp(head, path), 0);
  EXPECT_EQ(*tail, (const char)'\0');
}

TEST_F(SplitFunctionTest, WhenOnlyFileIsGiven) {
  const char* path = "CMakeLists.txt";
  Split(head, tail, path);

  EXPECT_EQ(*head, (const char)'\0');
  EXPECT_EQ(std::strcmp(tail, path), 0);
}

TEST_F(SplitFunctionTest, WhenANormalFilePathIsUsed) {
  const char* path = "/foo/bar/foo/buzz.hh";
  Split(head, tail, path);

  EXPECT_EQ(std::strcmp(head, "/foo/bar/foo"), 0);
  EXPECT_EQ(std::strcmp(tail, "buzz.hh"), 0);
}

class ProtectedGetCurrentWorkingDirFunctionTest : public ::testing::Test {
 protected:
  char buffer[100];
};

TEST_F(ProtectedGetCurrentWorkingDirFunctionTest,
       WhenOnlyRootNodeIsGivenAsAbspath) {
  const char* dir = "/";

  char* cwd_ = _GetCurrentWorkingDir(dir, buffer, sizeof(buffer));

  EXPECT_EQ(std::strcmp(buffer, dir), 0);
  EXPECT_EQ(std::strcmp(buffer, cwd_), 0);
}

TEST_F(ProtectedGetCurrentWorkingDirFunctionTest, WhenOnlyFileIsUsedAsAbspath) {
  const char* dir = "CMakeLists.txt";
  const char* cwd = "";

  char* cwd_ = _GetCurrentWorkingDir(dir, buffer, sizeof(buffer));

  EXPECT_EQ(std::strcmp(buffer, cwd), 0);
  EXPECT_EQ(std::strcmp(buffer, cwd_), 0);
}

TEST_F(ProtectedGetCurrentWorkingDirFunctionTest,
       WhenANormalFilePathIsUsedAsAbspath) {
  const char* dir = "/foo/bar/foo/buzz.hh";
  const char* cwd = "/foo/bar/foo";

  char* cwd_ = _GetCurrentWorkingDir(dir, buffer, sizeof(buffer));

  EXPECT_EQ(std::strcmp(buffer, cwd), 0);
  EXPECT_EQ(std::strcmp(buffer, cwd_), 0);
}

TEST_F(ProtectedGetCurrentWorkingDirFunctionTest, TestWhenFILEMacroIsUsed) {
  char head[100], tail[100];
  Split(head, tail, __FILE__);

  char* cwd_ = GetCurrentWorkingDir(buffer, sizeof(buffer));

  EXPECT_EQ(std::strcmp(buffer, head), 0);
  EXPECT_EQ(std::strcmp(cwd_, buffer), 0);
}

class JoinFunctionTest : public ::testing::Test {
 protected:
  char buffer[100];
};

TEST_F(JoinFunctionTest, WhenMultipleAbsPathsAreGiven) {
  const char* joined_path = Join(sizeof(buffer), buffer, 2, "/C", "/cjson");
  EXPECT_EQ(std::strcmp(buffer, "/cjson"), 0) << "buffer: " << buffer << '\n';
}

TEST_F(JoinFunctionTest, WhenOneFilePathIsUsedAfterAAbsPath) {
  const char* joined_path =
      Join(sizeof(buffer), buffer, 4, "C", "cjson", "/cjson", "CMakeLists.txt");

  EXPECT_EQ(std::strcmp(buffer, "/cjson/CMakeLists.txt"), 0)
      << "buffer: " << buffer << '\n';
}

class NormalizeFunctionTest : public ::testing::Test {
 protected:
  char buffer[100];
};

TEST_F(NormalizeFunctionTest, WhenNormalPathIsUsed) {
  const char* path = "/foo/bar/foo/buzz.hh";
  strcpy(buffer, path);
  Normalize(buffer);
  EXPECT_EQ(std::strcmp(buffer, path), 0) << "buffer: " << buffer << '\n';
}

TEST_F(NormalizeFunctionTest, WhenSingleDotIsUsedInPath) {
  const char* path = "/foo/./bar/foo/buzz.hh";
  strcpy(buffer, path);
  Normalize(buffer);
  EXPECT_EQ(std::strcmp(buffer, "/foo/bar/foo/buzz.hh"), 0)
      << "buffer: " << buffer << '\n';
}

TEST_F(NormalizeFunctionTest, WhenDoubleDotsAreUsedInPath) {
  const char* path = "/foo/../bar/foo/buzz.hh";
  strcpy(buffer, path);
  Normalize(buffer);
  EXPECT_EQ(std::strcmp(buffer, "/bar/foo/buzz.hh"), 0)
      << "buffer: " << buffer << '\n';
}

class ProtectedAbsPathFuntionTest : public ::testing::Test {
 protected:
  char buffer[100];
};

TEST_F(ProtectedAbsPathFuntionTest, WhenCurrentFileNameIsUsed) {
  char path[100];
  strcpy(path, "foo.hh");
  char* abs_path = _AbsPath("/foo/bar/foo/buzz.hh", buffer, path);
  EXPECT_EQ(std::strcmp(buffer, "/foo/bar/foo/foo.hh"), 0)
      << "buffer: " << buffer << '\n';
}
