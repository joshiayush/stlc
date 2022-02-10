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

// clang-format off
#define VECTOR_RESIZE_FAILURE       0
#define VECTOR_RESIZE_SUCCESS       !VECTOR_RESIZE_FAILURE
#define VECTOR_RESIZE_NOT_REQUIRED  ((VECTOR_RESIZE_FAILURE | VECTOR_RESIZE_SUCCESS) << VECTOR_RESIZE_SUCCESS)
// clang-format on

#define VECTOR_COPY_SUCCESS VECTOR_RESIZE_SUCCESS
#define VECTOR_COPY_FAILURE VECTOR_RESIZE_FAILURE

#ifdef __cplusplus
extern "C" {
#endif

// `Vector` is a container for our dynamic array.  It holds the actual array
// data, the size of the data and the capacity.
//
// `Vector` structure creates a generic container for dynamic arrays that means
// any type of array can be stored inside of the `Vector` container.
typedef struct Vector {
  void** data;
  size_t size;
  // data contains space for `capacity` elements.  The number currently in use
  // is `size`. Invariants:
  //     0 <= size <= capacity
  //     data == NULL implies size == capacity == 0
  size_t capacity;
} Vector;

// Returns an initialized instance of `Vector` of length `VECTOR_DEFAULT_SIZE`.
//
// `VECTOR_DEFAULT_SIZE` is used to compute the capacity of the `Vector`
// instance.
//
// Memory blocks allocated for `Vector` instances are allocated using the
// `capacity` calculated by the given `size` i.e., `VECTOR_DEFAULT_SIZE`.
Vector VectorDefAlloc();

// Returns an initialized instance of `Vector` of `length`.
//
// Memory blocks allocated for `Vector` instance will match the value of
// `capacity` which we calculate using the function
// `ComputeVectorBufferCapacity()`.
Vector VectorAlloc(const size_t size);

// Re-allocates the free store space occupied by the `Vector` container.
//
// This function re-allocates the `Vector` instance either by expanding the size
// in place (if available) or by moving the entire container to a new address.
//
// Function returns:
//  * `VECTOR_RESIZE_NOT_REQUIRED` if the given size is smaller than the
//    capacity,
//  * `VECTOR_RESIZE_SUCCESS` if the re-allocation was successful, or
//  * `VECTOR_RESIZE_FAILURE` if the re-allocation failed.
u_int8_t VectorResize(Vector* const vector, const size_t size);

// Copies `src` to `dest`.
//
// This function will not make the copies of the values stored inside of the
// `src` vector but will create a new list of pointers pointing to the values
// inside `src` vector.
//
// This is mainly used when `src` instance is stored in the `stack` while
// the values inside of it are stored in the `free-store` and you don't want
// to lose the memory when `src` goes out of scope; thus it's better to copy
// the entire `src` vector into a new vector that is dynamically allocated.
u_int8_t VectorCopy(Vector* const dest, Vector* const src);

// Clears up the `Vector` container and allocates fresh space for data elements.
//
// It does not free the free-store occupied by the `Vector` elements use
// `VectorFreeDeep()` for it.
void VectorClear(Vector* const vector);

// Frees up the free-store space occupied by the `Vector` container.
//
// It does not free the free-store occupied by the `Vector` elements use
// `VectorFreeDeep()` for it.
void VectorFree(Vector* const vector);

// Frees up the free store space occupied by the `Vector` container.
//
// This function frees up the free store space occupied by the `Vector`
// container and the elements in the `Vector` container so if you use this
// function keep this in mind that you`ll also lose access to the elements
// somewhere else in the free store pointed by the `Vector` container, like
// "lose lose".
//
// THE MAIN CAVEAT of using this function is that the elements stored in this
// `vector` must be dynamically allocated otherwise you might get a error:
//          free(): double free detected in tcache 2
//          Aborted (core dumped)
void VectorFreeDeep(Vector* const vector);

#ifdef __cplusplus
}
#endif

#include "data/vector/accessors.h"
#include "data/vector/iterators.h"
#include "data/vector/modifiers.h"

#endif  // CJSON_INCLUDE_DATA_VECTOR_VECTOR_H_
