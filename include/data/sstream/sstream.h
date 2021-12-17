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

#ifndef CJSON_INCLUDE_DATA_SSTREAM_SSTREAM_H_
#define CJSON_INCLUDE_DATA_SSTREAM_SSTREAM_H_

#include <sys/types.h>

#define SSTREAM_DEFAULT_SIZE (1 << 2)

#define REALLOC_FAILURE 0
#define REALLOC_SUCCESS !REALLOC_FAILURE
#define REALLOC_NOT_REQUIRED \
  ((REALLOC_FAILURE | REALLOC_SUCCESS) << REALLOC_SUCCESS)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief stringstream structure is a container for our string stream. It holds
 * the actual data, the length of the string and the capacity.
 *
 * @param data String.
 * @param length String length.
 * @param capacity String capacity.
 */
typedef struct stringstream {
  char* data;
  size_t length;
  size_t capacity;
} stringstream;

/**
 * @brief Function allocates a stringstream instance using a default string
 * length value in case the length is not given.
 *
 * @return stringstream - Heap allocated instance.
 */
stringstream stringstream_def_alloc();

/**
 * @brief Function allocates a stringstream instance using the string size @p
 * length.
 *
 * @param[in] length Bytes to allocate for the string.
 * @return stringstream - Heap allocated instance.
 */
stringstream stringstream_alloc(const size_t length);

/**
 * @brief Function allocates a stringstream instance using a const char*
 * instance.
 *
 * @param[in] string String to copy to stringstream instance.
 * @return stringstream - Heap allocated instance.
 */
stringstream stringstream_str_alloc(const char* string);

/**
 * @brief Function reallocates the free store space occupied by the stringstream
 * data.
 *
 * @param[in] sstream stringstream instance.
 * @param[in] length New space to occupy in free store.
 * @return REALLOC_FAILURE If function failed in reallocating free store space.
 * @return REALLOC_SUCCESS If function succeed in reallocating free store space.
 * @return REALLOC_NOT_REQUIRED If the stringstream instance already has enough
 * space.
 */
__uint8_t stringstream_realloc(stringstream* const sstream,
                               const size_t length);

/**
 * @brief Function deallocates the memory occupied by the stringstream instance
 * in the free store.
 *
 * @param[in] sstream stringstream instance.
 */
void stringstream_dealloc(stringstream* const sstream);

#ifdef __cplusplus
}
#endif

#endif  // CJSON_INCLUDE_DATA_SSTREAM_SSTREAM_H_
