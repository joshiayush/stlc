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

// Computes the capacity of the ``StringStream`` instance using the Python list
// resize routine so that the following evaluates to true:
//      0 <= length <= capacity
static void _ComputeStringStreamBufferCapacity(const size_t length,
                                               size_t* const capacity) {
  *capacity = (length >> 3) + (length < 9 ? 3 : 6);
  *capacity += length;
}

// Default string stream allocator in case the length is not known.
//
// This function will initialize a ``StringStream`` container with a initial
// ``length`` of ``SSTREAM_DEFAULT_SIZE``.
StringStream StringStreamDefAlloc() {
  return StringStreamAlloc(SSTREAM_DEFAULT_SIZE);
}

// Allocates ``StringStream`` instance of given length.
StringStream StringStreamAlloc(const size_t length) {
  StringStream sstream = {.data = (void*)0, .length = 0, .capacity = 0};
  size_t capacity;
  _ComputeStringStreamBufferCapacity(length, &capacity);
  if (sstream.data = (char*)malloc(capacity * sizeof(char))) {
    sstream.capacity = capacity;
    _TERMINATE_STRING_STREAM_BUFFER(sstream);
  }
  return sstream;
}

// Allocates ``StringStream`` from a ``const char*`` C-String.
//
// The length of the ``StringStream`` instance will be the number of items from
// the first element to the first NULL byte in the string.
StringStream StringStreamStrAlloc(const char* string) {
  return StringStreamStrNAlloc(string, strlen(string));
}

// Allocates ``n`` from a ``const char*`` C-String into a ``StringStream``
// instance.
//
// This function will create a ``StringStream`` instance by copying all the
// characters from the given C-String to the ``data`` member inside
// ``StringStream`` instance regardless of any ``NULL`` byte on the way.
StringStream StringStreamStrNAlloc(const char* string, const size_t length) {
  StringStream sstream = StringStreamAlloc(length);
  if (sstream.capacity) {
    memcpy(sstream.data, string, length * sizeof(char));
    sstream.length = length;
  }
  return sstream;
}

// Reallocates the free store space occupied by the ``StringStream`` instance.
//
// This function reallocates the ``StringStream`` instance either by expanding
// the size in place (if available) or by moving the entire container to a new
// address.
//
// ``length`` is the new length of the container; if less than the capacity of
// the container then this function will simply return the value of macro
// ``SSTREAM_REALLOC_NOT_REQUIRED`` or if greater than the capacity of the
// container then will return ``SSTREAM_REALLOC_SUCCESS`` on success or
// ``SSTREAM_REALLOC_FAILURE`` on failure.
__uint8_t StringStreamRealloc(StringStream* const sstream,
                              const size_t length) {
  if (length <= sstream->capacity)
    return SSTREAM_REALLOC_NOT_REQUIRED;
  size_t capacity;
  _ComputeStringStreamBufferCapacity(length, &capacity);
  char* data = sstream->data;
  sstream->data = (char*)realloc(sstream->data, capacity * sizeof(char));
  if (!sstream->data) {
    if (!(sstream->data = (char*)malloc(capacity * sizeof(char)))) {
      sstream->data = data;
      return SSTREAM_REALLOC_FAILURE;
    }
    memcpy(sstream->data, data, sstream->length * sizeof(char));
    free(data);
  }
  sstream->capacity = capacity;
  return SSTREAM_REALLOC_SUCCESS;
}

// Deallocates the memory occupied by the ``StringStream`` instance.
void StringStreamDealloc(StringStream* const sstream) {
  sstream->length = 0;
  sstream->capacity = 0;
  free(sstream->data);
  sstream->data = (void*)0;
}
