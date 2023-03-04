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

// The module includes functions for determining whether a path is absolute,
// splitting a path into its head and tail components, getting the current
// working directory, joining multiple path components, normalizing paths, and
// getting the absolute path to a given path.

#include <stdint.h>

#include "bool.h"

#ifdef __cplusplus
extern "C" {
#endif

// The `IsAbsPath()` function takes a `const char*` path as input and returns a
// boolean indicating whether the path is an absolute path. The function
// determines whether a path is absolute by checking if the first element of the
// path is `/`. This function does support "Windows-specific" paths too.
bool_t IsAbsPath(const char* const path);

// The `Split()` function takes a `const char*` path as input and returns two
// `char*` pointers: "head" and "tail". The function splits the input path into
// two separate components, where everything before the last root node `/` is
// considered the "head" and everything after is considered the "tail". The
// function does not normalize the path before splitting it and instead simply
// separates the "head" and "tail" components. It is recommended to use the
// `Normalize()` function to normalize the path before calling this function.
void Split(char* const head, char* const tail, const char* const path);

// The `_GetCurrentWorkingDir()` function is a private function that takes in a
// `const char* abspath`, which is used to create the absolute path to the
// current working directory. The function returns a `char*` pointer to the
// input buffer after copying the current working directory path into it. The
// size parameter determines how many bytes from the current working directory
// path will be copied into the buffer. This function is not intended to be
// called directly by users.
char* _GetCurrentWorkingDir(const char* const abspath, char* const buffer,
                            const size_t size);

// The `GetCurrentWorkingDir()` function is a wrapper over the private function
// `_GetCurrentWorkingDir()`. It returns a `char*` pointer to the input buffer
// after copying the current working directory path into it. The function takes
// two arguments: "buffer" and "size". The buffer parameter is a pointer to a
// buffer where the current working directory path will be stored. The size
// parameter determines how many bytes from the current working directory path
// will be copied into the buffer. This function provides a `__FILE__` path when
// the user wants the current working directory path.
#define GetCurrentWorkingDir(buffer, size) \
  _GetCurrentWorkingDir(__FILE__, buffer, size)

// The `Join()` function takes a variable number of path components as input and
// returns a `char*` pointer to the input buffer after joining the path
// components together. The function inserts `/` as needed to join the path
// components. If any component is an absolute path, all previous path
// components will be discarded. An empty last part will result in a path that
// ends with a separator.
char* Join(const size_t bufsize, char* const buffer, const u_int64_t paths,
           ...);

// The `Normalize()` function takes a `char*` path as input and returns a
// `char*` pointer to the normalized path. The function normalizes the path by
// eliminating double slashes, dots, double dots, and any other unnecessary
// components.
char* Normalize(char* const path);

// The `_AbsPath()` function is a private function that takes in a `const char*`
// abspath and a `char*` path as input. The function returns a `char*` pointer
// to the input buffer after concatenating the current working directory with
// the given path and normalizing the resulting path. The function uses
// `abspath` to find the absolute path to the current working directory and then
// joins that with the given path. This function is not intended to be called
// directly by users.
char* _AbsPath(const char* const abspath, char* const buffer, char* const path);

// The `AbsPath()` function is a macro wrapper around the `_AbsPath()` function,
// which passes `__FILE__` as its first argument. This allows the user to easily
// get the absolute path to a given path argument from the current working
// directory.
#define AbsPath(buffer, path) _AbsPath(__FILE__, buffer, path)

#ifdef __cplusplus
}
#endif

#endif  // CJSON_INCLUDE_INTERNAL_FS_H_
