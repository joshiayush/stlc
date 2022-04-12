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

#ifndef CJSON_INCLUDE_INTERNAL_FS_H_
#define CJSON_INCLUDE_INTERNAL_FS_H_

#include <stdint.h>

#include "bool.h"

#ifdef __cplusplus
extern "C" {
#endif

// Returns a `bool_t` value in case the path given is an absolute path.
// Unfortunately this does not support Windows specific paths yet.
//
// The given `path` is consider to be a absolute path if and only if that path
// has `/` as its first element.
bool_t IsAbsPath(const char* const path);

// Splits the `path` into two separate components `head` and `tail` where
// everything before the last root node `/` is considerd the `head` and
// everything after is considered the `tail` of that path.
//
// Note: This function is not going to normalize the `path` before separating it
// into components rather will just simply separate the `head` and the `tail`
// component out of the `path` without normalizing it.  Use `NormaPath` to
// normalize the path.
void Split(char* const head, char* const tail, const char* const path);

// Returns a pointer to the `buffer` buffer itself after copying the current
// working directory path in it.  This function will only copy `size` bytes from
// the current working directory to the `buffer` given so make sure to create
// enough room for the `buffer` before actually storing a large current working
// directory path in it.
//
// This function is private to the module and takes in a `abspath` argument
// which is used to create the absolute path to the current working directory.
char* _GetCurrentWorkingDir(const char* const abspath, char* const buffer,
                            const size_t size);

// Wrapper over the private function `_GetCurrentWorkingDir()` to provide a
// `__FILE__` path when user wants the current working directory path.
//
// We can't get a absolute path to the current working directory unless we have
// a absolute path to the file the program is running in.
#define GetCurrentWorkingDir(buffer, size) \
  _GetCurrentWorkingDir(__FILE__, buffer, size)

// Joins two or more pathname components, inserting `/` as needed.
// Unfortunately, this will not work in Windows.
//
// Note: If any component is absolute path, all previous path components will be
// discarded.  An empty last part will result in a path that ends with a
// separator.
char* Join(const size_t bufsize, char* const buffer, const u_int64_t paths,
           ...);

// Normalizes path, eliminating double slashes, dots, double dots, etc.
char* Normalize(char* const path);

// Returns a pointer to the `buffer` after copying a absolute path to the given
// `path` from the current working directory.  Basically this function will just
// concatenate the current working directory with the given `path` and will
// normalize the path before returning the pointer to the resulting path.
//
// `abspath` is the path to the file from where the function is called, using
// this path we find the absolute path to the current working directory and
// joins that with the given `path`.
char* _AbsPath(const char* const abspath, char* const buffer, char* const path);

// Wrapper over the private function `_AbsPath()` to provide `__FILE__` as its
// first argument when user wants to get the absolute path to the given `path`.
#define AbsPath(buffer, path) _AbsPath(__FILE__, buffer, path)

#ifdef __cplusplus
}
#endif

#endif  // CJSON_INCLUDE_INTERNAL_FS_H_
