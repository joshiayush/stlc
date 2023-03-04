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

#include "stlc-string.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
  comps[ncomps] = NULL;  // At the end append a `NULL` pointer.
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
