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

#ifndef CJSON_INCLUDE_DATA_SSTREAM_MODIFIERS_H_
#define CJSON_INCLUDE_DATA_SSTREAM_MODIFIERS_H_

#include <sys/types.h>

#include "bytes.h"
#include "data/sstream/sstream.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Terminates a string inside of the stringstream instance at '\0'.
 *
 * @param[in] sstream stringstream instance.
 *
 * @note This macro is private to the module sstream.
 */
#define _terminate(sstream)                       \
  do {                                            \
    if ((sstream).data && (sstream).capacity)     \
      (sstream).data[(sstream).length] = NULLCHR; \
  } while (0)

/**
 * @brief Concatenates a newly formatted string onto the existing string in
 * stringstream instance.
 *
 * @param[in] sstream stringstream instance.
 * @param[in] format Formatted string to concatenate.
 * @param[in] ... Arguments to substitute the formatted string with.
 */
void stringstream_concat(stringstream* const sstream, const char* format, ...);

/**
 * @brief Concatenates data of known length on to the existing data instance of
 * stringstream instance.
 *
 * @param[in] sstream stringstream instance.
 * @param[in] data String to concatenate the new data on.
 * @param[in] length String to concatenate.
 */
void stringstream_read(stringstream* const sstream, const void* data,
                       const size_t length);

/**
 * @brief Impedes the position of the terminator character ('\0') by length, or
 * if the length is greater than the data length of stringstream instance,
 * places the terminator character ('\0') at beginning.
 *
 * @param[in] sstream stringstream instance.
 * @param[in] length Number of elements to impede the terminator character
 * ('\0') by.
 */
void stringstream_retreat(stringstream* const sstream, const size_t length);

#ifdef __cplusplus
}
#endif

#endif  // CJSON_INCLUDE_DATA_SSTREAM_MODIFIERS_H_
