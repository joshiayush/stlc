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

#include "data/d_array/iterators.h"

#include <sys/types.h>

#include "bool.h"

d_array_iterator d_array_iterator_new(const d_array* const darray) {
  d_array_iterator darrayit;
  darrayit.data = darray;
  darrayit.cur_idx = 0;
  return darrayit;
}

void* d_array_iterator_next(d_array_iterator* it) {
  return it->cur_idx < it->data->size ? it->data->data[it->cur_idx++]
                                      : (void*)0;
}

/**
 * @brief Function executes the given predicate on each element of the @a
 * d_array instance.
 *
 * @param darray @a d_array instance.
 * @param pred Function to execute on each element of the @a d_array instance.
 */
void d_array_map(d_array* const darray, void (*pred)(const void* const elem)) {
  for (size_t i = 0; i < darray->size; ++i)
    pred(darray->data[i]);
}

/**
 * @brief Function to check if for any value in the @a d_array instance the
 * given predicate evaluates to true or not.
 *
 * @param darray @a d_array instance.
 * @param pred Function to execute on each element of the @a d_array instance.
 * @return bool_t @a TRUE if predicate returns @a TRUE for any value in @a
 * d_array instance.
 */
bool_t d_array_any(d_array* const darray,
                   bool_t (*pred)(const void* const elem)) {
  for (size_t i = 0; i < darray->size; ++i)
    if (pred(darray->data[i]))
      return TRUE;
  return FALSE;
}

/**
 * @brief Function to check if for all values in the @a d_array instance the
 * given predicate evaluates to true or not.
 *
 * @param darray @a d_array instance.
 * @param pred Function to execute on each element of the @a d_array instance.
 * @return bool_t @a TRUE if predicate returns @a TRUE for every value in @a
 * d_array instance.
 */
bool_t d_array_all(d_array* const darray,
                   bool_t (*pred)(const void* const elem)) {
  for (size_t i = 0; i < darray->size; ++i)
    if (!pred(darray->data[i]))
      return FALSE;
  return TRUE;
}
