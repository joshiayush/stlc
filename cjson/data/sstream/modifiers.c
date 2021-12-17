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

#include "data/sstream/modifiers.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "data/sstream/accessors.h"
#include "data/sstream/sstream.h"

/**
 * @brief Concatenates a newly formatted string onto the existing string in
 * stringstream instance.
 *
 * @param[in] sstream stringstream instance.
 * @param[in] format Formatted string to concatenate.
 * @param[in] ... Arguments to substitute the formatted string with.
 */
void stringstream_concat(stringstream* const sstream, const char* format, ...) {
  va_list args;
  va_start(args, format);
  size_t avail = stringstream_avail(*sstream);
  size_t format_size = vsnprintf(sstream->data + sstream->length,
                                 avail * sizeof(char), format, args);
  va_end(args);
  if (stringstream_realloc(sstream, sstream->length + format_size) ==
      REALLOC_SUCCESS) {  // vsnprintf() has overflow protection, so if this
                          // condition evaluates to true that means vsnprintf()
                          // did not concatenate the new string properly; this
                          // clause takes care of that
    va_start(args, format);
    avail = stringstream_avail(*sstream);
    format_size = vsnprintf(sstream->data + sstream->length,
                            avail * sizeof(char), format, args);
    va_end(args);
  }
  sstream->length += format_size;
  _terminate(*sstream);
}

/**
 * @brief Concatenates data of known length on to the existing data instance of
 * stringstream instance.
 *
 * @param[in] sstream stringstream instance.
 * @param[in] data String to concatenate the new data on.
 * @param[in] length String to concatenate.
 */
void stringstream_read(stringstream* const sstream, const void* data,
                       const size_t length) {
  (void)stringstream_realloc(sstream, sstream->length + length);
  memcpy(sstream->data + sstream->length, data, length);
  sstream->length += length;
  _terminate(*sstream);
}

/**
 * @brief Impedes the position of the terminator character ('\0') by length, or
 * if the length is greater than the data length of stringstream instance,
 * places the terminator character ('\0') at beginning.
 *
 * @param[in] sstream stringstream instance.
 * @param[in] length Number of elements to impede the terminator character
 * ('\0') by.
 */
void stringstream_retreat(stringstream* const sstream, const size_t length) {
  if (!sstream->length || !sstream->capacity)
    return;
  sstream->length = length >= sstream->length ? 0 : sstream->length - length;
  _terminate(*sstream);
}
