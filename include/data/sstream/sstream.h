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

#ifndef _CJSON_DATA_SSTREAM_SSTREAM_H_
#define _CJSON_DATA_SSTREAM_SSTREAM_H_

#include <sys/types.h>

#define SSTREAM_DEFAULT_SIZE (1 << 2)

#define REALLOC_FAILURE 0
#define REALLOC_SUCCESS !REALLOC_FAILURE
#define REALLOC_NOT_REQUIRED \
  ((REALLOC_FAILURE | REALLOC_SUCCESS) << REALLOC_SUCCESS)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief stringstream structure is a container for our string stream. It holds
 * the actual data, the length of the string and the capacity.
 *
 * @param data String.
 * @param length String length.
 * @param capacity String capacity.
 */
typedef struct stringstream {
  char* data;
  size_t length;
  size_t capacity;
} stringstream;

/**
 * @brief Structure used in the wrapper function _stringstream_var_args_alloc()
 * to allocate a default string length of SSTREAM_DEFAULT_SIZE in case the
 * string length is not given explicitly.
 *
 * @param length String length.
 */
typedef struct _stringstream_alloc_args {
  size_t length;
} _stringstream_alloc_args;

/**
 * @brief Wrapper around the function _stringstream_var_args_alloc to provide
 * variadic arguments to the _stringstream_alloc_args instance while
 * instantiating one with a length argument.
 *
 * The C standard allows specifying named parameters multiple times, the later
 * override, so we leverage this power of C and pass a default .length argument
 * in case none is given.
 *
 * @param[in] __VA_ARGS__ Variadic arguments.
 *
 * @example stringstream_alloc();
 *  stringstream_alloc(.length=20);
 */
#define stringstream_alloc(...) \
  _stringstream_var_args_alloc( \
      (_stringstream_alloc_args){.length = SSTREAM_DEFAULT_SIZE, __VA_ARGS__})

/**
 * @brief Function allocates a stringstream instance using a
 * _stringstream_alloc_args struct to populate the string length argument with a
 * default value in case the length is not given.
 *
 * This is a workaround to pass in default parameters to function
 * _stringstream_alloc().
 *
 * @param[in] sstream_var_args _stringstream_alloc_args instance.
 * @return stringstream - Heap allocated instance.
 */
stringstream _stringstream_var_args_alloc(
    const struct _stringstream_alloc_args sstream_var_args);

/**
 * @brief Function allocates a stringstream instance using the string size @p
 * length.
 *
 * @param[in] length Bytes to allocate for the string.
 * @return stringstream - Heap allocated instance.
 */
stringstream _stringstream_alloc(const size_t length);

/**
 * @brief Function allocates a stringstream instance using a const char*
 * instance.
 *
 * @param[in] string String to copy to stringstream instance.
 * @return stringstream - Heap allocated instance.
 */
stringstream stringstream_str_alloc(const char* string);

/**
 * @brief Function reallocates the free store space occupied by the stringstream
 * data.
 *
 * @param[in] sstream stringstream instance.
 * @param[in] length New space to occupy in free store.
 * @return REALLOC_FAILURE If function failed in reallocating free store space.
 * @return REALLOC_SUCCESS If function succeed in reallocating free store space.
 * @return REALLOC_NOT_REQUIRED If the stringstream instance already has enough
 * space.
 */
__uint8_t stringstream_realloc(stringstream* const sstream,
                               const size_t length);

/**
 * @brief Function deallocates the memory occupied by the stringstream instance
 * in the free store.
 *
 * @param[in] sstream stringstream instance.
 */
void stringstream_dealloc(stringstream* const sstream);

/**
 * @brief Returns a iterator to the beginning of the data instance of the
 * stringstream instance.
 *
 * @param[in] sstream stringstream instance.
 * @return const char* pointer to the beginning of the data instance.
 */
const char* begin(const stringstream* const sstream);

/**
 * @brief Returns a iterator to the end of the data instance of the stringstream
 * instance.
 *
 * @param[in] sstream stringstream instance.
 * @return const char* pointer to the end of the data instance.
 */
const char* end(const stringstream* const sstream);

#ifdef __cplusplus
}
#endif

#endif