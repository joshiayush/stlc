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

#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "bytes.h"

static const char kPathSeparator = '/';

// Returns a `bool_t` value in case the path given is an absolute path.
// Unfortunately this does not support Windows specific paths yet.
//
// The given `path` is consider to be a absolute path if and only if that path
// has `/` as its first element.
bool_t IsAbsPath(const char* const path) { return *path == kPathSeparator; }

// Splits the `path` into two separate components `head` and `tail` where
// everything before the last root node `/` is considerd the `head` and
// everything after is considered the `tail` of that path.
//
// Note: This function is not going to normalize the `path` before separating it
// into components rather will just simply separate the `head` and the `tail`
// component out of the `path` without normalizing it.  Use `NormaPath` to
// normalize the path.
void Split(char* const head, char* const tail, const char* const path) {
  size_t node_r_idx = __SIZE_MAX__;
  size_t pathlen = strlen(path);
  for (size_t i = pathlen - 1; i >= 0 && i != __SIZE_MAX__; --i) {
    if (path[i] == kPathSeparator) {
      node_r_idx = i;
      break;
    }
  }
  if (node_r_idx == __SIZE_MAX__) {
    strcpy(head, "");
    strcpy(tail, path);
    return;
  }
  strncpy(head, path, node_r_idx + 1);
  head[(node_r_idx + 1)] = nullchr;
  if (node_r_idx != 0)
    head[node_r_idx] = nullchr;
  for (size_t i = 0; i < (pathlen - (node_r_idx + 1)); ++i)
    tail[i] = path[(node_r_idx + 1) + i];
  tail[(pathlen - (node_r_idx + 1))] = nullchr;
}

// Returns a pointer to the `buffer` buffer itself after copying the current
// working directory path in it.  This function will only copy `size` bytes from
// the current working directory to the `buffer` given so make sure to create
// enough room for the `buffer` before actually storing a large current working
// directory path in it.
//
// This function is private to the module and takes in a `abspath` argument
// which is used to create the absolute path to the current working directory.
char* _GetCurrentWorkingDir(const char* const abspath, char* const buffer,
                            const size_t size) {
  char* head = (char*)malloc(sizeof(char) * size);
  char* tail = (char*)malloc(sizeof(char) * size);
  Split(head, tail, abspath);
  strncpy(buffer, head, size);
  free(head);
  free(tail);
  return buffer;
}

// Joins two or more pathname components, inserting `/` as needed.
// Unfortunately, this will not work in Windows.
//
// Note: If any component is absolute path, all previous path components will be
// discarded.  An empty last part will result in a path that ends with a
// separator.
char* Join(const size_t bufsize, char* const buffer, const u_int64_t paths,
           ...) {
  for (size_t i = 0; i < bufsize; ++i)
    buffer[i] = nullchr;
  va_list args;
  va_start(args, paths);
  for (u_int64_t i = 0; i < paths; ++i) {
    if (kPathSeparator == '/')
      strcat(buffer, "/");
    else
      strcat(buffer, "\\");
    const char* p = va_arg(args, char*);
    if (IsAbsPath(p)) {
      for (size_t i = 0; i < bufsize; ++i)
        buffer[i] = nullchr;
      strcat(buffer, p);
    } else {
      strcat(buffer, p);
    }
    size_t buflen = strlen(buffer);
    size_t j = 0;
    for (; j < buflen; ++j) {
      if (buffer[i] == kPathSeparator)
        break;
    }
    if (j == 1) {
      for (size_t i = 1; i < buflen; ++i)
        buffer[i - 1] = buffer[i];
    }
  }
  va_end(args);
  return buffer;
}

// Normalizes path, eliminating double slashes, dots, double dots, etc.
char* NormPath(char* const path) { return path; }

// Returns a pointer to the `buffer` after copying a absolute path to the given
// `path` from the current working directory.  Basically this function will just
// concatenate the current working directory with the given `path` and will
// normalize the path before returning the pointer to the resulting path.
//
// `abspath` is the path to the file from where the function is called, using
// this path we find the absolute path to the current working directory and
// joins that with the given `path`.
char* _AbsPath(const char* const abspath, char* const buffer,
               char* const path) {
  return buffer;
}
