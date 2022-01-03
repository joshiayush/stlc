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

#ifndef CJSON_INCLUDE_DATA_VECTOR_VECTOR_H_
#define CJSON_INCLUDE_DATA_VECTOR_VECTOR_H_

#include <sys/types.h>

#define VECTOR_DEFAULT_SIZE (1 << 2)

#define VECTOR_RESIZE_FAILURE 0
#define VECTOR_RESIZE_SUCCESS !VECTOR_RESIZE_FAILURE
#define VECTOR_RESIZE_NOT_REQUIRED \
  ((VECTOR_RESIZE_FAILURE | VECTOR_RESIZE_SUCCESS) << VECTOR_RESIZE_SUCCESS)

#ifdef __cplusplus
extern "C" {
#endif

// 'Vector' is a container for our dynamic array.  It holds the actual array
// data, the size of the data and the capacity.
//
// 'Vector' structure creates a generic container for dynamic arrays that means
// any type of array can be stored inside of the 'Vector' container.
typedef struct Vector {
  void** data;
  size_t size;
  // data contains space for 'capacity' elements.  The number
  // currently in use is 'size'.
  // Invariants:
  //     0 <= size <= capacity
  //     data == NULL implies size == capacity == 0
  size_t capacity;
} Vector;

// Default dynamic array allocator in case the length is not known.
//
// This function will initialize a 'Vector' container with a initial
// 'length' of 'VECTOR_DEFAULT_SIZE'.
Vector VectorDefAlloc();

// Allocate 'Vector' container of given size.
Vector VectorAlloc(const size_t size);

// Re-allocates the free store space occupied by the 'Vector' container.
//
// This function re-allocates the 'Vector' instance either by expanding the
// size in place (if available) or by moving the entire container to a new
// address.
//
// 'size' is the new size of the container; if less than the capacity of the
// container then this function will simply return the value of macro
// 'VECTOR_RESIZE_NOT_REQUIRED' or if greater than the capacity of the
// container then will return 'VECTOR_RESIZE_SUCCESS' on success or
// 'VECTOR_RESIZE_FAILURE' on failure.
__uint8_t VectorResize(Vector* const vector, const size_t size);

// Clears up the 'Vector' container and allocates fresh space for data elements.
void VectorClear(Vector* const vector);

// Frees up the free store space occupied by the 'Vector' container.
//
// This function frees up the free store space occupied by the 'Vector'
// container.
//
// Note: This function does not free up the space occupied by the container
// elements in the free store.
void VectorFree(Vector* const vector);

// Frees up the free store space occupied by the 'Vector' container.
//
// This function frees up the free store space occupied by the 'Vector'
// container and the elements in the 'Vector' container so if you use this
// function keep this in mind that you'll also lose access to the elements
// somewhere else in the free store pointed by the 'Vector' container, like
// "lose lose".
void VectorFreeDeep(Vector* const vector);

#ifdef __cplusplus
}
#endif

#include "data/vector/accessors.h"
#include "data/vector/iterators.h"
#include "data/vector/modifiers.h"

#endif  // CJSON_INCLUDE_DATA_VECTOR_VECTOR_H_
