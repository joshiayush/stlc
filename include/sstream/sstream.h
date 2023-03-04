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

#ifndef CJSON_INCLUDE_DATA_SSTREAM_SSTREAM_H_
#define CJSON_INCLUDE_DATA_SSTREAM_SSTREAM_H_

#include <sys/types.h>

#define SSTREAM_DEFAULT_SIZE (1 << 2)

#define SSTREAM_REALLOC_FAILURE 0
#define SSTREAM_REALLOC_SUCCESS !SSTREAM_REALLOC_FAILURE
#define SSTREAM_REALLOC_NOT_REQUIRED                   \
  ((SSTREAM_REALLOC_FAILURE | SSTREAM_REALLOC_SUCCESS) \
   << SSTREAM_REALLOC_SUCCESS)

#ifdef __cplusplus
extern "C" {
#endif

// Container for our string streams.
//
// Holds the actual data, the length of the string and the capacity of the
// StringStream container.
typedef struct StringStream {
  char* data;
  size_t length;

  // data contains space for `capacity` elements.  The number currently in use
  // is length. Invariants:
  //     0 <= length <= capacity
  //     data == NULL implies length == capacity == 0
  size_t capacity;
} StringStream;

// Returns a initialized `StringStream` instance.
//
// This function will initialize a `StringStream` container with a initial
// `length` of `_SSTREAM_DEFAULT_SIZE`.
//
// Memory blocks allocated for `StringStream` instances are allocated using the
// `capacity` calculated by the given `length` i.e., `SSTREAM_DEFAULT_SIZE`.
StringStream StringStreamAlloc(void);

// Returns a initialized `StringStream` instance of given length.
//
// Memory blocks allocated for `StringStream` instance will match the value of
// `capacity` which we calculate using the function
// `ComputeStringStreamBufferCapacity()`.
StringStream StringStreamNAlloc(const size_t length);

// Returns a initialized `StringStream` instance from a `const char*` C-String.
//
// The length of the `StringStream` instance will be the number of items from
// the first element to the first NULL byte in the string.
//
// Use `StringStreamStrNAlloc` to allocate a `StringStream` instance with a
// C-String containing `NULL` bytes in between.
StringStream StringStreamStrAlloc(const char* string);

// Returns a initialized `StringStream` instance from a `const char*` C-String
// by copying `n` bytes to `StringStream.data`.
//
// Copies all the characters from the given C-String to the `data` member inside
// `StringStream` instance regardless of any `NULL` byte on the way.
StringStream StringStreamStrNAlloc(const char* string, const size_t n);

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
u_int8_t StringStreamRealloc(StringStream* const sstream, const size_t length);

// Deallocates the memory occupied by the `StringStream` instance.
void StringStreamDealloc(StringStream* const sstream);

#ifdef __cplusplus
}
#endif

#include "sstream/accessors.h"
#include "sstream/fileio.h"
#include "sstream/iterators.h"
#include "sstream/modifiers.h"
#include "sstream/printers.h"

#endif  // CJSON_INCLUDE_DATA_SSTREAM_SSTREAM_H_
