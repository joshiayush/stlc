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

#include "fs.h"

#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "bool.h"
#include "bytes.h"
#include "arch.h"
#include "string.h"

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
//
// Note: If any component is a absolute path, all previous path components will
// be discarded.  An empty last part will result in a path that ends with a
// separator.
char* Join(const size_t bufsize, char* const buffer, const u_int64_t paths,
           ...) {
  for (size_t i = 0; i < bufsize; ++i)
    buffer[i] = nullchr;
  va_list args;
  va_start(args, paths);
  for (u_int64_t i = 0; i < paths; ++i) {
#ifdef CJSON_OS_WINDOWS
    strcat(buffer, "\\");
#else
    strcat(buffer, "/");
#endif
    const char* p = va_arg(args, char*);
    // If the component we just took out is a absolute node then we must reset
    // the buffer with null bytes so to overwrite the previous path.
    if (IsAbsPath(p)) {
      for (size_t i = 0; i < bufsize; ++i)
        buffer[i] = nullchr;
    }
    // Append the component onto the given buffer.
    strcat(buffer, p);
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
char* Normalize(char* path) {
  const char* empty = "";
  const char* dot = ".";
  const char* dotdot = "..";
#ifdef CJSON_OS_WINDOWS
  const char* sep = "\\";
#else
  const char* sep = "/";
#endif

  // Path becomes dot (.) in case the given `path` is empty.
  if (strcmp(path, empty) == 0) {
    strcpy(path, dot);
    return path;
  }

#ifdef CJSON_OS_WINDOWS
  char** comps = SplitStr(path, "\\");
#else
  char** comps = SplitStr(path, "/");
#endif

  // We need to calculate the number of components we have in our path string so
  // to later allocate a buffer of the right size to store our new components
  // for normalized path string.
  size_t ncomps = 0;
  for (const char** ptr = (const char**)comps; *ptr != NULL; ++ptr)
    ++ncomps;

  // Now allocate `ncomps` block of memory each of size (char*) to store our
  // normalized components.
  char** new_comps = (char**)calloc(ncomps, sizeof(char*));
  char** ptr = new_comps;
  while (*comps != NULL) {
    if (strcmp(*comps, dot) == 0 || strcmp(*comps, empty) == 0) {
      // If the component is just a `dot` or a `empty` string then we just skip
      // it.
      ++comps;
      continue;
    } else if (strcmp(*comps, dotdot) != 0) {
      // If the component is not a `dotdot` then we just copy the address to the
      // `new_comps`.
      *ptr++ = *comps;
    } else {
      // If the component is a `dotdot` then we have to go back one component so
      // that we can store the comming component after `dotdot` into its right
      // place.
      *ptr-- = NULL;
    }
    ++comps;
  }

  size_t pathlen = strlen(path);
  for (size_t i = 0; i < pathlen; ++i)
    path[i] = '\0';  // Rest path to null bytes.

  for (size_t i = 0; i < ncomps; ++i) {
    if (new_comps[i] == NULL)
      continue;
    // Append separator and new components in our `path` string.
    strcat(path, sep);
    strcat(path, new_comps[i]);
  }

  for (char** ptr = comps; *ptr != NULL; ++ptr)
    free(*ptr);
  free(new_comps);

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
  size_t size = 0;
  for (const char* ptr = abspath; *ptr != '\0'; ++ptr)
    ++size;
  char* head = (char*)malloc(sizeof(char) * size);
  char* tail = (char*)malloc(sizeof(char) * size);
  Split(head, tail, abspath);
  Join(size, buffer, 2, head, path);
  free(head);
  free(tail);
  return Normalize(buffer);
}
