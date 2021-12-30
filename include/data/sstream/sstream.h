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

#define SSTREAM_REALLOC_FAILURE 0
#define SSTREAM_REALLOC_SUCCESS !SSTREAM_REALLOC_FAILURE
#define SSTREAM_REALLOC_NOT_REQUIRED                   \
  ((SSTREAM_REALLOC_FAILURE | SSTREAM_REALLOC_SUCCESS) \
   << SSTREAM_REALLOC_SUCCESS)

#ifdef __cplusplus
extern "C" {
#endif

// Container for our string streams.  It holds the actual data, the length of
// the string and the capacity of the StringStream container.
typedef struct StringStream {
  char* data;
  size_t length;
  // data contains space for 'capacity' elements.  The number
  // currently in use is length.
  // Invariants:
  //     0 <= length <= capacity
  //     data == NULL implies length == capacity == 0
  size_t capacity;
} StringStream;

// Default string stream allocator in case the length is not known.
//
// This function will initialize a 'StringStream' container with a initial
// 'length' of '_SSTREAM_DEFAULT_SIZE'.
StringStream StringStreamDefAlloc();

// Allocates 'StringStream' instance of given length.
StringStream StringStreamAlloc(const size_t length);

// Allocates 'StringStream' from a 'const char*' C-String.
//
// The length of the 'StringStream' instance will be the number of items from
// the first element to the first NULL byte in the string.
StringStream StringStreamStrAlloc(const char* string);

// Allocates 'n' from a 'const char*' C-String into a 'StringStream' instance.
//
// This function will create a 'StringStream' instance by copying all the
// characters from the given C-String to the 'data' member inside 'StringStream'
// instance regardless of any 'NULL' byte on the way.
StringStream StringStreamStrNAlloc(const char* string, const size_t n);

// Reallocates the free store space occupied by the 'StringStream' instance.
//
// This function reallocates the 'StringStream' instance either by expanding the
// size in place (if available) or by moving the entire container to a new
// address.
//
// 'length' is the new length of the container; if less than the capacity of the
// container then this function will simply return the value of macro
// 'SSTREAM_REALLOC_NOT_REQUIRED' or if greater than the capacity of the
// container then will return 'SSTREAM_REALLOC_SUCCESS' on success or
// 'SSTREAM_REALLOC_FAILURE' on failure.
__uint8_t StringStreamRealloc(StringStream* const sstream, const size_t length);

// Deallocates the memory occupied by the 'StringStream' instance.
void StringStreamDealloc(StringStream* const sstream);

#ifdef __cplusplus
}
#endif

#endif  // CJSON_INCLUDE_DATA_SSTREAM_SSTREAM_H_
