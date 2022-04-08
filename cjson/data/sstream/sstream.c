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

#include "data/sstream/sstream.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "data/sstream/modifiers.h"

// Computes the capacity of the ``StringStream`` instance using the Python list
// resize routine so that the following evaluates to true:
//      0 <= length <= capacity
static void ComputeStringStreamBufferCapacity(const size_t length,
                                              size_t* const capacity) {
  *capacity = (length >> 0x03) + (length < 0x09 ? 0x03 : 0x06);
  *capacity += length;
}

// Returns a initialized `StringStream` instance.
//
// This function will initialize a `StringStream` container with a initial
// `length` of `_SSTREAM_DEFAULT_SIZE`.
//
// Memory blocks allocated for `StringStream` instances are allocated using the
// `capacity` calculated by the given `length` i.e., `SSTREAM_DEFAULT_SIZE`.
StringStream StringStreamAlloc() {
  return StringStreamNAlloc(SSTREAM_DEFAULT_SIZE);
}

// Returns a initialized `StringStream` instance of given length.
//
// Memory blocks allocated for `StringStream` instance will match the value of
// `capacity` which we calculate using the function
// `ComputeStringStreamBufferCapacity()`.
StringStream StringStreamNAlloc(const size_t length) {
  StringStream sstream = {.data = (void*)0, .length = 0, .capacity = 0};
  size_t capacity;
  ComputeStringStreamBufferCapacity(length, &capacity);
  if (sstream.data = (char*)malloc(capacity * sizeof(char))) {
    sstream.capacity = capacity;
    _TERMINATE_STRING_STREAM_BUFFER(sstream);
  }
  return sstream;
}

// Returns a initialized `StringStream` instance from a `const char*` C-String.
//
// The length of the `StringStream` instance will be the number of items from
// the first element to the first NULL byte in the string.
//
// Use `StringStreamNAlloc` to allocate a `StringStream` instance with a
// C-String containing `NULL` bytes in between.
StringStream StringStreamStrAlloc(const char* string) {
  return StringStreamStrNAlloc(string, strlen(string));
}

// Returns a initialized `StringStream` instance from a `const char*` C-String
// by copying `n` bytes to `StringStream.data`.
//
// Copies all the characters from the given C-String to the `data` member inside
// `StringStream` instance regardless of any `NULL` byte on the way.
StringStream StringStreamStrNAlloc(const char* string, const size_t length) {
  StringStream sstream = StringStreamNAlloc(length);
  if (sstream.capacity) {
    memcpy(sstream.data, string, length * sizeof(char));
    sstream.length = length;
    _TERMINATE_STRING_STREAM_BUFFER(sstream);
  }
  return sstream;
}

// Reallocates the free-store space occupied by the given `StringStream`
// instance.
//
// This function reallocates the `StringStream` instance either by expanding the
// size in place (if available) or by moving the entire container to a new
// address.
//
// Function returns:
//  * `SSTREAM_REALLOC_NOT_REQUIRED` if the given length is smaller than the
//    capacity,
//  * `SSTREAM_REALLOC_SUCCESS` if the re-allocation was successful, or
//  * `SSTREAM_REALLOC_FAILURE` if the re-allocation failed.
u_int8_t StringStreamRealloc(StringStream* const sstream, const size_t length) {
  if (length <= sstream->capacity)
    return SSTREAM_REALLOC_NOT_REQUIRED;
  size_t capacity;
  ComputeStringStreamBufferCapacity(length, &capacity);
  char* data = sstream->data;
  if (!(sstream->data =
            (char*)realloc(sstream->data, capacity * sizeof(char)))) {
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

// Deallocates the memory occupied by the `StringStream` instance.
void StringStreamDealloc(StringStream* const sstream) {
  sstream->length = 0;
  sstream->capacity = 0;
  free(sstream->data);
  sstream->data = (void*)0;
}
