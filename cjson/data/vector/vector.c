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

#include "data/vector/vector.h"

#include <stdlib.h>
#include <sys/types.h>

// Computes the capacity of the 'Vector' instance using the Python list resize
// routine so that the following evaluates to true:
//      0 <= size <= capacity
static void _ComputeVectorBufferCapacity(const size_t size,
                                         size_t* const capacity) {
  *capacity = (size >> 3) + (size < 9 ? 3 : 6);
  *capacity += size;
}

// Default dynamic array allocator in case the length is not known.
//
// This function will initialize a 'Vector' container with a initial
// 'length' of 'VECTOR_DEFAULT_SIZE'.
Vector VectorDefAlloc() { return VectorAlloc(VECTOR_DEFAULT_SIZE); }

// Allocate 'Vector' container of given size.
Vector VectorAlloc(const size_t size) {
  Vector vector = {.data = (void*)0, .size = size, .capacity = 0};
  size_t capacity;
  _ComputeVectorBufferCapacity(size, &capacity);
  if (vector.data = (void**)malloc(capacity * sizeof(void*)))
    vector.capacity = capacity;
  return vector;
}

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
__uint8_t VectorResize(Vector* const vector, const size_t size) {
  if (size <= vector->capacity)
    return VECTOR_RESIZE_NOT_REQUIRED;
  size_t capacity;
  _ComputeVectorBufferCapacity(size, &capacity);
  void** data = vector->data;
  vector->data = (void**)realloc(vector->data, capacity * sizeof(void*));
  if (!vector->data) {
    if (!(vector->data = (void**)malloc(capacity * sizeof(void*)))) {
      vector->data = data;
      return VECTOR_RESIZE_FAILURE;
    }
    for (size_t i = 0; i < vector->size; ++i)
      vector->data[i] = data[i];
    free(data);
  }
  vector->capacity = capacity;
  return VECTOR_RESIZE_SUCCESS;
}

// Clears up the 'Vector' container and allocates fresh space for data elements.
void VectorClear(Vector* const vector) {
  free(vector->data);
  vector->data = (void**)malloc(vector->capacity * sizeof(void*));
}

// Frees up the free store space occupied by the 'Vector' container.
//
// This function frees up the free store space occupied by the 'Vector'
// container.
//
// Note: This function does not free up the space occupied by the container
// elements in the free store.
void VectorFree(Vector* const vector) {
  vector->size = 0;
  vector->capacity = 0;
  free(vector->data);
  vector->data = (void*)0;
}

// Frees up the free store space occupied by the 'Vector' container.
//
// This function frees up the free store space occupied by the 'Vector'
// container and the elements in the 'Vector' container so if you use this
// function keep this in mind that you'll also lose access to the elements
// somewhere else in the free store pointed by the 'Vector' container, like
// "lose lose".
//
// THE MAIN CAVEAT of using this function is that the elements stored in this
// 'vector' must be dynamically allocated otherwise you might get a error:
//          free(): double free detected in tcache 2
//          Aborted (core dumped)
void VectorFreeDeep(Vector* const vector) {
  for (size_t i = 0; i < vector->capacity; ++i)
    free(vector->data[i]);
  vector->size = 0;
  vector->capacity = 0;
  vector->data = (void*)0;
}
