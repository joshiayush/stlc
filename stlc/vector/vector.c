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

#include "vector/vector.h"

#include <stdlib.h>
#include <sys/types.h>

// Computes the capacity of the `Vector` instance using the Python list resize
// routine so that the following evaluates to true:
//      0 <= size <= capacity
static void ComputeVectorBufferCapacity(const size_t size,
                                        size_t* const capacity) {
  *capacity = (size >> 0x03) + (size < 0x09 ? 0x03 : 0x06);
  *capacity += size;
}

// Initialises instance of `Vector` of `length`.
//
// Memory blocks allocated for `Vector` instance will match the value of
// `capacity` which we calculate using the function
// `ComputeVectorBufferCapacity()`.
void VectorInit(Vector* const vector, const ssize_t size) {
  vector->data = (void*)0;
  vector->size = 0;
  vector->capacity = 0;
  size_t capacity;
  ComputeVectorBufferCapacity(size > -1 ? size : VECTOR_DEFAULT_SIZE,
                              &capacity);
  if ((vector->data = (void**)malloc(capacity * sizeof(void*))))
    vector->capacity = capacity;
}

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
u_int8_t VectorResize(Vector* const vector, const size_t size) {
  if (size <= vector->capacity) return VECTOR_RESIZE_NOT_REQUIRED;
  size_t capacity;
  ComputeVectorBufferCapacity(size, &capacity);
  void** data = vector->data;
  vector->data = (void**)realloc(vector->data, capacity * sizeof(void*));
  if (!vector->data) {
    if (!(vector->data = (void**)malloc(capacity * sizeof(void*)))) {
      vector->data = data;
      return VECTOR_RESIZE_FAILURE;
    }
    for (size_t i = 0; i < vector->size; ++i) vector->data[i] = data[i];
    free(data);
  }
  vector->capacity = capacity;
  return VECTOR_RESIZE_SUCCESS;
}

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
u_int8_t VectorCopy(Vector* const dest, Vector* const src) {
  if (dest == NULL) return VECTOR_COPY_FAILURE;
  if (src == NULL) {
    dest->data = NULL;
    dest->size = 0;
    ComputeVectorBufferCapacity(dest->size, &dest->capacity);
    return VECTOR_COPY_SUCCESS;
  }
  if (src->size > dest->size)
    if (VectorResize(dest, src->size) == VECTOR_RESIZE_FAILURE)
      return VECTOR_COPY_FAILURE;
  for (size_t i = 0; i < src->size; ++i) *(dest->data + i) = *(src->data + i);
  dest->size = src->size;
  return VECTOR_COPY_SUCCESS;
}

// Clears up the `Vector` container and allocates fresh space for data elements.
//
// It does not free the free-store occupied by the `Vector` elements use
// `VectorFreeDeep()` for it.
void VectorClear(Vector* const vector) {
  free(vector->data);
  vector->data = (void**)malloc(vector->capacity * sizeof(void*));
  vector->size = 0;
  ComputeVectorBufferCapacity(vector->size, &vector->capacity);
}

// Frees up the free-store space occupied by the `Vector` container.
//
// It does not free the free-store occupied by the `Vector` elements use
// `VectorFreeDeep()` for it.
void VectorFree(Vector* const vector) {
  vector->size = 0;
  vector->capacity = 0;
  free(vector->data);
  vector->data = (void*)0;
}

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
void VectorFreeDeep(Vector* const vector) {
  for (size_t i = 0; i < vector->capacity; ++i) free(vector->data[i]);
  vector->size = 0;
  vector->capacity = 0;
  vector->data = (void*)0;
}
