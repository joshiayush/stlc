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

#include "data/vector/modifiers.h"

#include <sys/types.h>

#include "data/vector/vector.h"

// Function to insert elmenet at a specific index inside the 'Vector' container.
//
// Function inserts element at a specific index inside the 'Vector' container
// provided that the index is in bounds.
void VectorInsert(Vector* const vector, void* const elem, const size_t idx) {
  if (idx > vector->size)
    return;
  if (vector->size == vector->capacity)
    VectorResize(vector, vector->size + 1);
  for (size_t i = vector->size; i > idx; --i)
    vector->data[i] = vector->data[i - 1];
  vector->data[idx] = elem;
  ++(vector->size);
}

// Function to insert the given element at the very first index shifting the
// rest of the array elements towards the last.
void VectorUnshift(Vector* const vector, void* const elem) {
  VectorInsert(vector, elem, 0);
}

// Function to insert the given element at the very last index of the 'Vector'
// buffer.
void VectorPush(Vector* const vector, void* const elem) {
  VectorInsert(vector, elem, vector->size);
}

// Returns the address of the element located at the index 'idx' while
// over-writing element's position.
void* VectorDelete(Vector* const vector, const size_t idx) {
  if (idx >= vector->size)
    return (void*)0;
  void* elem = vector->data[idx];
  for (size_t i = idx + 1; i < vector->size; ++i)
    vector->data[i - 1] = vector->data[i];
  vector->data[vector->size - 1] = (void*)0;
  --(vector->size);
  return elem;
}

// Returns the very first element of a 'Vector' instance and then over writes
// the memory block.
void* VectorShift(Vector* const vector) { return VectorDelete(vector, 0); }

// Returns the very last element of a 'Vector' instance and then over writes the
// memory block.
void* VectorRemove(Vector* const vector) {
  return VectorDelete(vector, vector->size - 1);
}
