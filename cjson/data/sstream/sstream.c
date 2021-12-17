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

#include "data/sstream/sstream.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "data/sstream/modifiers.h"

/**
 * @brief Function allocates a stringstream instance using a default string
 * length value in case the length is not given.
 *
 * @return stringstream - Heap allocated instance.
 */
stringstream stringstream_def_alloc() {
  return stringstream_alloc(SSTREAM_DEFAULT_SIZE);
}

/**
 * @brief Function allocates a stringstream instance using the string size @p
 * length.
 *
 * @param[in] length Bytes to allocate for the string.
 * @return stringstream - Heap allocated instance.
 */
stringstream stringstream_alloc(const size_t length) {
  stringstream sstream = {.data = (void*)0, .length = length, .capacity = 0};
  size_t capacity = 1;
  while (capacity < length)
    capacity <<= 1;
  if (sstream.data = malloc(capacity * sizeof(char))) {
    sstream.capacity = capacity;
    _terminate(sstream);
  }
  return sstream;
}

/**
 * @brief Function allocates a stringstream instance using a const char*
 * instance.
 *
 * @param[in] string String to copy to stringstream instance.
 * @return stringstream - Heap allocated instance.
 */
stringstream stringstream_str_alloc(const char* string) {
  size_t length = strlen(string);
  stringstream sstream = stringstream_alloc(length);
  if (sstream.capacity) {
    memcpy(sstream.data, string, length * sizeof(char));
    sstream.length = length;
  }
  return sstream;
}

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
                               const size_t length) {
  if (length <= sstream->capacity)
    return REALLOC_NOT_REQUIRED;
  size_t capacity = sstream->capacity ? sstream->capacity : 1;
  while (capacity <= length + 1)
    capacity <<= 1;
  char* data = sstream->data;
  sstream->data = realloc(sstream->data, capacity * sizeof(char));
  if (!sstream->data) {
    if (!(sstream->data = malloc(capacity * sizeof(char)))) {
      sstream->data = data;
      return REALLOC_FAILURE;
    }
    memcpy(sstream->data, data, sstream->length * sizeof(char));
    free(data);
  }
  sstream->capacity = capacity;
  return REALLOC_SUCCESS;
}

/**
 * @brief Function deallocates the memory occupied by the stringstream instance
 * in the free store.
 *
 * @param[in] sstream stringstream instance.
 */
void stringstream_dealloc(stringstream* const sstream) {
  sstream->length = 0;
  sstream->capacity = 0;
  free(sstream->data);
  sstream->data = (void*)0;
}
