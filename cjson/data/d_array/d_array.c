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

#include "data/d_array/d_array.h"

#include <stdlib.h>
#include <sys/types.h>

/**
 * @brief Function allocates a d_array instance using a _d_array_alloc_args
 * struct to populate the array size argument with a default value in case the
 * size is not given.
 *
 * This is a workaround to pass in default parameters to function
 * _d_array_alloc().
 *
 * @param[in] darray_var_args _d_array_alloc_args instance.
 * @return d_array - Heap allocated instance.
 */
d_array _d_array_var_args_alloc(const _d_array_alloc_args darray_var_args) {
  return _d_array_alloc(darray_var_args.size);
}

/**
 * @brief Function allocates a d_array instance in the free store of the given
 * size @p size.
 *
 * @param[in] size Size to allocate in the free store.
 * @return d_array - Heap allocated instance.
 */
d_array _d_array_alloc(const size_t size) {
  d_array darray = {.data = (void*)0, .size = size, .capacity = 0};
  size_t capacity = 1;
  while (capacity < size)
    capacity <<= 1;
  if (darray.data = malloc(capacity * sizeof(void*)))
    darray.capacity = capacity;
  return darray;
}

/**
 * @brief Function reallocates the free store space occupied by the d_array
 * instance.
 *
 * @param[in] darray d_array instance.
 * @param[in] size New space to occupy in free store.
 * @return RESIZE_FAILURE If function failed in reallocating free store space.
 * @return RESIZE_SUCCESS If function succeed in reallocating free store space.
 * @return RESIZE_NOT_REQUIRED If the stringstream instance already has enough
 * space.
 */
__uint8_t d_array_resize(d_array* const darray, const size_t size) {
  if (size <= darray->capacity)
    return RESIZE_NOT_REQUIRED;
  size_t capacity = darray->capacity ? darray->capacity : 1;
  while (capacity < size)
    capacity <<= 1;
  void** data = darray->data;
  darray->data = realloc(darray->data, capacity * sizeof(void*));
  if (!darray->data) {
    if (!(darray->data = malloc(capacity * sizeof(void*)))) {
      darray->data = data;
      return RESIZE_FAILURE;
    }
    for (size_t i = 0; i < darray->size; ++i)
      darray->data[i] = data[i];
    free(data);
  }
  darray->capacity = capacity;
  return RESIZE_SUCCESS;
}

/**
 * @brief Function clears up the @a d_array container and allocates fresh space
 * for data elements.
 *
 * @param darray @a d_array instance.
 */
void d_array_clear(d_array* const darray) {
  free(darray->data);
  darray->size = D_ARRAY_DEFAULT_SIZE;
  size_t capacity = 1;
  while (capacity < darray->size)
    capacity <<= 1;
  darray->data = malloc(capacity * sizeof(void*));
  darray->capacity = capacity;
}

/**
 * @brief Function frees up the free store space occupied by the @a d_array
 * container.
 *
 * @details This function frees up the free store space occupied by the @a
 * d_array container. Note, this function does not free up the space occupied by
 * the container elements in the free store.
 *
 * @param darray @a d_array instance.
 */
void d_array_free(d_array* const darray) {
  darray->size = 0;
  darray->capacity = 0;
  free(darray->data);
  darray->data = (void*)0;
}

/**
 * @brief Functions frees up the free store space occupied by the @a d_array
 * container and the elements.
 *
 * @details This function frees up the free store space occupied by the @a
 * d_array container and the elements in the @a d_array container. So if you use
 * this function keep this in mind that you'll also lose access to the elements
 * somewhere else in the free store pointed by the @a d_array container, like
 * "lose lose".
 *
 * @param darray @a d_array instance.
 */
void d_array_free_deep(d_array* const darray) {
  for (size_t i = 0; i < darray->capacity; ++i)
    free(darray->data[i]);
  darray->size = 0;
  darray->capacity = 0;
  free(darray->data);
  darray->data = (void*)0;
}
