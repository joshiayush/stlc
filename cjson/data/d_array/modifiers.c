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