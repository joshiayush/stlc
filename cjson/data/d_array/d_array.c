// Copyright (C) 2021  Ayush Joshi
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.
//
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <https://www.gnu.org/licenses/>.

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
d_array _d_array_var_args_alloc(
    const _d_array_alloc_args darray_var_args) {
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