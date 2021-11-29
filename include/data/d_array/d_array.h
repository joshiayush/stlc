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

#ifndef _CJSON_DATA_D_ARRAY_D_ARRAY_H_
#define _CJSON_DATA_D_ARRAY_D_ARRAY_H_

#include <sys/types.h>

#define D_ARRAY_DEFAULT_SIZE (1 << 2)

#define RESIZE_FAILURE 0
#define RESIZE_SUCCESS !RESIZE_FAILURE
#define RESIZE_NOT_REQUIRED \
  ((RESIZE_FAILURE | RESIZE_SUCCESS) << RESIZE_SUCCESS)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief d_array structure is a container for our dynamic array. It holds the
 * actual array data, the size of the data and the capacity.
 *
 * @param data Pointer to the dynamic array.
 * @param size Size of the dynamic array.
 * @param capacity Element holding capacity of the dynamic array.
 */
typedef struct d_array {
  void** data;
  size_t size;
  size_t capacity;
} d_array;

/**
 * @brief Structure used in the wrapper function _d_array_var_args_alloc() to
 * allocate a default array size of D_ARRAY_DEFAULT_SIZE in case the array size
 * is not given explicitly.
 *
 * @param size Array size.
 */
typedef struct _d_array_alloc_args {
  size_t size;
} _d_array_alloc_args;

/**
 * @brief Wrapper around the function _d_array_var_args_alloc() to provide
 * variadic arguments to the _d_array_alloc_args instance while instantiating
 * one with the size argument.
 *
 * The C standard allows specifying named parameters multiple times, the later
 * override, so we leverage this power of C and pass a default .size argument in
 * case none is given.
 *
 * @param[in] ... Variadic argument list.
 *
 * @example
 * @code
 *  d_array_alloc();
 *  d_array_alloc(.size=20);
 * @endcode
 */
#define d_array_alloc(...) \
  _d_array_var_args_alloc( \
      (_d_array_alloc_args){.size = D_ARRAY_DEFAULT_SIZE, __VA_ARGS__})

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
d_array _d_array_var_args_alloc(const _d_array_alloc_args darray_var_args);

/**
 * @brief Function allocates a d_array instance in the free store of the given
 * size @p size.
 *
 * @param[in] size Size to allocate in the free store.
 * @return d_array - Heap allocated instance.
 */
d_array _d_array_alloc(const size_t size);

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
__uint8_t d_array_resize(d_array* const darray, const size_t size);

/**
 * @brief Function clears up the @a d_array container and allocates fresh space
 * for data elements.
 *
 * @param darray @a d_array instance.
 */
void d_array_clear(d_array* const darray);

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
void d_array_free(d_array* const darray);

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
void d_array_free_deep(d_array* const darray);

#ifdef __cplusplus
}
#endif

#endif