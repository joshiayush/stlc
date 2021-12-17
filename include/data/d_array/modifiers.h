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

#ifndef CJSON_INCLUDE_DATA_D_ARRAY_MODIFIERS_H_
#define CJSON_INCLUDE_DATA_D_ARRAY_MODIFIERS_H_

#include <sys/types.h>

#include "data/d_array/d_array.h"

#ifdef __cplusplus
extern "C" {
#endif

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
                    const size_t idx);

/**
 * @brief Function inserts the given element at the very first index shifting
 * the rest of the array elements towards the last.
 *
 * @param darray @a d_array instance.
 * @param elem Element to insert at the very first index.
 */
void d_array_unshift(d_array* const darray, const void* const elem);

/**
 * @brief Function inserts the given element at the very last index of the @a
 * d_array array.
 *
 * @param darray @a d_array instance.
 * @param elem Element to insert at the very last index.
 */
void d_array_push(d_array* const darray, const void* const elem);

/**
 * @brief Returns the address of the element located at index @p idx while
 * over-writing element's position.
 *
 * @param darray @a d_array instance.
 * @param idx Index of element.
 * @return void* to element that was once located at index @p idx.
 */
void* d_array_delete(d_array* const darray, const size_t idx);

/**
 * @brief Returns the very first element of a @a d_array array and then over
 * writes the memory block.
 *
 * @param darray @a d_array instance.
 * @return void* to element that was once located at the very first index.
 */
void* d_array_shift(d_array* const darray);

/**
 * @brief Returns the very last element of a @a d_array array and then over
 * writes the memory block.
 *
 * @param darray @a d_array instance.
 * @return void* to element that was once located at the very last index.
 */
void* d_array_remove(d_array* const darray);

#ifdef __cplusplus
}
#endif

#endif  // CJSON_INCLUDE_DATA_D_ARRAY_MODIFIERS_H_
