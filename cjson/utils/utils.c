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
#include "internal/arch.h"

// Splits a string at `sep`.
//
// Returns a heap-allocated array of pointers containing components separated at
// `sep`.
//
// Caller must free the component string inside the returned array of pointers
// and the returned array of pointers too.
char** SplitStr(const char* str, const char* sep) {
  size_t ncomps = 0;

  // Length of the given separator string (it can be a single letter string or a
  // multiple letter string).
  size_t seplen = strlen(sep);
  size_t strlen_ = strlen(str);

  // Compute number of total components we have in the given path string i.e.,
  // `str`.
  for (size_t i = 0; i < strlen_; ++i) {
    // If we find a separator string at the current position in the given path
    // then increment `ncomps`.
    if ((strncmp((str + i), sep, seplen) == 0))
      ++ncomps;
  }

  // If the given path string have a separator at the end that means we've
  // computed one more component then we should so we must decrement `ncomps`.
  if (strncmp((str + strlen_ - seplen), sep, seplen) == 0)
    --ncomps;

  // Allocate `ncomps` block of memory, each of a size of `char*` to store the
  // splited components.
  char** comps = (char**)calloc(ncomps, sizeof(char*));

  // Declare `beg` and `end` pointers to point to the beginning and the end of a
  // component in the given path string.
  const char* beg = NULL;
  const char* end = NULL;

  // Number of component in the given path string that we are taking out as a
  // component for our `comps` pointer.
  size_t comp_pos = 0;

  for (size_t i = 0; i < strlen_; ++i) {
    // Because `comp_pos` starts with 0, we must break the for loop once the
    // `comp_pos` is equals to `ncomps`.
    if (ncomps == comp_pos)
      break;

    // If `end` pointer is not pointing to a `NULL` value then our new beginning
    // must start from `end + seplen`.
    if (end)
      beg = (end++ + seplen);
    else
      beg =
          (str + i);  // Otherwise, our new beginning must start from `str + i`.

    // Our `beg` pointer is set now find the position of the `end` pointer.
    for (size_t j = 0; j < strlen_; ++j) {
      if (end && (strncmp(end, sep, seplen) == 0))
        break;
      end = (beg + j);
    }

    // Continue if we couldn't find a valid `end` pointer.  `end` must always be
    // at a greater position than `beg`.
    if ((end - beg) <= 0)
      continue;

    // Allocate memory for the current component because we are about to take it
    // out.
    char* str_ = (char*)malloc((end - beg) * sizeof(char));
    char* ptr = str_;
    for (beg; beg != end; ++beg)
      *ptr++ = *beg;  // Copy every element from `beg` to `end` to the newly
                      // allocated memory.
    *ptr = '\0';      // At the end append a `end-of-string` character.

    // Save the address of the dynamic memory where our current component lies.
    comps[comp_pos++] = str_;
  }

  return comps;
}

// Joins the list of `strings` into a single string at `sep`.
void JoinStr(char* const buffer, const char* sep, const char* const* const strs,
             size_t nstrs) {
  (*buffer) = '\0';  // Reset the buffer.
  for (size_t i = 0; i < nstrs; ++i) {
    if (i > 0)
      strcat(buffer, sep);  // Only append the separator if we have added at
                            // least one component to the `buffer`.
    strcat(buffer, strs[i]);
  }
}

#ifdef CJSON_OS_WINDOWS
static const char kPathSeparator = '\\';
#else
static const char kPathSeparator = '/';
#endif

// Returns a `bool_t` value in case the path given is an absolute path.
// Unfortunately this does not support Windows specific paths yet.
//
// The given `path` is consider to be a absolute path if and only if that path
// has `/` as its first element.
bool_t IsAbsPath(const char* const path) {
#ifdef CJSON_OS_WINDOWS
  if ((*path >= 0x41 && *path <= 0x5A) || (*path >= 0x61 && *path <= 0x7A))
    return *(path + 1) == ':' && *(path + 2) == kPathSeparator;
#else
  return *path == kPathSeparator;
#endif
}

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

  // We always want to start from the end of the `path` string and we want to
  // find the first character which is `kPathSeparator` so that we can remember
  // the position of the last `kPathSeparator` from the beginning in
  // `node_r_idx`.
  for (size_t i = pathlen - 1; i >= 0 && i != __SIZE_MAX__; --i) {
    if (path[i] == kPathSeparator) {
      node_r_idx = i;  // We only want the last position of `kPathSeparator` in
                       // the given `path` string.
      break;
    }
  }

  // If we couldn't find a `kPathSeparator` in the given `path` string then we
  // know that the head component is missing so we just copy the whole `path`
  // string to the `tail` component and leave `head` component empty.
  if (node_r_idx == __SIZE_MAX__) {
    strcpy(head, "");
    strcpy(tail, path);
    return;
  }

  // If we successfully found a `kPathSeparator` in the given `path` string then
  // we copy everything upto `node_r_idx + 1` into the `head` component.
  strncpy(head, path, node_r_idx + 1);
  head[(node_r_idx + 1)] = nullchr;  // Append a `end-of-string` character.

  // Once we have taked out our `head` we don't want the node at the end of it
  // so we just overwrite the last character with a `end-of-string` character.
  if (node_r_idx != 0)
    head[node_r_idx] = nullchr;

  // Now we want to copy everything after the last `kPathSeparator` into the
  // `tail` component.
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
char* NormPath(char* const path) {
  const char* empty = "";
  const char* dot = ".";
  const char* dotdot = "..";
  const char* sep = "/";

  // Path becomes dot (.) in case the given `path` is empty.
  if (strcmp(path, empty) == 0) {
    strcpy(path, dot);
    return path;
  }

  return path;
}

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
