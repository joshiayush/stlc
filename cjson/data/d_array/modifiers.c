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

#include "data/d_array/modifiers.h"

#include <sys/types.h>

#include "data/d_array/d_array.h"

/**
 * @brief Function to insert element at a specific index in the @a d_array
 * container.
 *
 * @details Function inserts element at a specific index in the @a d_array
 * container provided that the index is in bounds.
 *
 * @param darray @a d_array instance.
 * @param elem Element to insert inside of the @a d_array container.
 * @param idx Index to insert at.
 */
void d_array_insert(d_array* const darray, const void* const elem,
                    const size_t idx) {
  if (idx > darray->size)
    return;
  if (darray->size == darray->capacity)
    d_array_resize(darray, darray->size + 1);
  for (size_t i = darray->size; i > idx; --i)
    darray->data[i] = darray->data[i - 1];
  darray->data[idx] = elem;
  ++(darray->size);
}

/**
 * @brief Functions inserts the given element at the very first index shifting
 * the rest of the array elements towards the last.
 *
 * @param darray @a d_array instance.
 * @param elem Element to insert at the very first index.
 */
void d_array_unshift(d_array* const darray, const void* const elem) {
  d_array_insert(darray, elem, 0);
}

/**
 * @brief Function inserts the given element at the very last index of the @a
 * d_array array.
 *
 * @param darray @a d_array instance.
 * @param elem Element to insert at the very last index.
 */
void d_array_push(d_array* const darray, const void* const elem) {
  d_array_insert(darray, elem, darray->size);
}

/**
 * @brief Returns the address of the element located at index @p idx while
 * over-writing element's position.
 *
 * @param darray @a d_array instance.
 * @param idx Index of element.
 * @return void* to element that was once located at index @p idx.
 */
void* d_array_delete(d_array* const darray, const size_t idx) {
  if (idx >= darray->size)
    return (void*)0;
  void* elem = darray->data[idx];
  for (size_t i = idx + 1; i < darray->size; ++i)
    darray->data[i - 1] = darray->data[i];
  darray->data[darray->size - 1] = (void*)0;
  --(darray->size);
  return elem;
}

/**
 * @brief Returns the very first element of a @a d_array array and then over
 * writes the memory block.
 *
 * @param darray @a d_array instance.
 * @return void* to element that was once located at the very first index.
 */
void* d_array_shift(d_array* const darray) { return d_array_delete(darray, 0); }

/**
 * @brief Returns the very last element of a @a d_array array and then over
 * writes the memory block.
 *
 * @param darray @a d_array instance.
 * @return void* to element that was once located at the very last index.
 */
void* d_array_remove(d_array* const darray) {
  return d_array_delete(darray, darray->size - 1);
}
