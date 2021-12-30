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

#ifndef CJSON_INCLUDE_DATA_VECTOR_MODIFIERS_H_
#define CJSON_INCLUDE_DATA_VECTOR_MODIFIERS_H_

#include <sys/types.h>

#include "data/vector/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function to insert elmenet at a specific index inside the 'Vector' container.
//
// Function inserts element at a specific index inside the 'Vector' container
// provided that the index is in bounds.
void VectorInsert(Vector* const vector, void* const elem, const size_t idx);

// Function to insert the given element at the very first index shifting the
// rest of the array elements towards the last.
void VectorUnshift(Vector* const vector, void* const elem);

// Function to insert the given element at the very last index of the 'Vector'
// buffer.
void VectorPush(Vector* const vector, void* const elem);

// Returns the address of the element located at the index 'idx' while
// over-writing element's position.
void* VectorDelete(Vector* const vector, const size_t idx);

// Returns the very first element of a 'Vector' instance and then over writes
// the memory block.
void* VectorShift(Vector* const vector);

// Returns the very last element of a 'Vector' instance and then over writes the
// memory block.
void* VectorRemove(Vector* const vector);

#ifdef __cplusplus
}
#endif

#endif  // CJSON_INCLUDE_DATA_VECTOR_MODIFIERS_H_
