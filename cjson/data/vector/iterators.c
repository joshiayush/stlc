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
// this software without spescific prior written permission.
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

#include "data/vector/iterators.h"

#include <sys/types.h>

#include "bool.h"
#include "data/vector/vector.h"

// Creates a new `VectorIterator` instance using a `Vector` instance.
VectorIterator VectorIteratorNew(Vector *const vector) {
  VectorIterator vector_itr;
  vector_itr.data = vector;
  vector_itr.cur_idx = 0;
  return vector_itr;
}

void *VectorIteratorNext(VectorIterator *itr) {
  return itr->cur_idx < itr->data->size ? itr->data->data[itr->cur_idx++]
                                        : (void *)0;
}

// Executes the given predicate on each element of the `Vector` instance.
void VectorMap(Vector *const vector, void (*pred)(const void *const elem)) {
  for (size_t i = 0; i < vector->size; ++i)
    pred(vector->data[i]);
}

// Checks if for any value in the `Vector` instance the given predicate
// evaluates to true or not.
//
// Elements can be of any type that evaluates to true when used as an expression
// inside of the `if` clause.
bool_t VectorAny(Vector *const vector, bool_t (*pred)(const void *const elem)) {
  for (size_t i = 0; i < vector->size; ++i)
    if (pred(vector->data[i]))
      return TRUE;
  return FALSE;
}

// Checks if for all of the values in the `Vector` instance the given predicate
// evaluates to true or not.
//
// Elements can be of any type that evaluates to true when used as an expression
// inside of the `if` clause.
bool_t VectorAll(Vector *const vector, bool_t (*pred)(const void *const elem)) {
  for (size_t i = 0; i < vector->size; ++i)
    if (!pred(vector->data[i]))
      return FALSE;
  return TRUE;
}
