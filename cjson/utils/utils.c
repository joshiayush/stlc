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

#include "utils/utils.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

static const char kPathSeparator = '/';

bool_t IsAbsPath(const char* const path) { return *path == kPathSeparator; }

void Split(char* const head, char* const tail, const char* const path) {
  ssize_t last_sep_idx = -1;

  if (path == NULL)
    return;

  size_t pathlen = strlen(path);
  for (size_t i = 0; i < pathlen; ++i)
    if (*path == kPathSeparator)
      last_sep_idx = i;

  strncpy(head, path, last_sep_idx + 1);
  *(head + (last_sep_idx + 1)) = '\0';
  if (*(path + (char)last_sep_idx + 1) == '\0') {
    *tail = '\0';
    return;
  } else {
    strncpy(tail, path + (char)last_sep_idx + 1, pathlen - last_sep_idx + 1);
    *(tail + (pathlen - last_sep_idx + 2)) = '\0';
  }
}

// char* _GetCurrentWorkingDir(char* const abspath, char* const buffer,
//                             const size_t size) {}
