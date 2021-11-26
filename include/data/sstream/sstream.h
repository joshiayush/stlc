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

#ifndef _DATA_SSTREAM_SSTREAM_H_
#define _DATA_SSTREAM_SSTREAM_H_

#include <sys/types.h>

#define SSTREAM_DEFAULT_SIZE (1 << 2)

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
  __uint32_t length;
  __uint32_t capacity;
} stringstream;

/**
 * @brief Function allocates a stringstream instance using the default string
 * size of SSTREAM_DEFAULT_SIZE.
 *
 * @return stringstream - Heap allocated instance.
 */
stringstream stringstream_alloc_default();

/**
 * @brief Function allocates a stringstream instance using the string size @p
 * length.
 *
 * @param[in] length Bytes to allocate for the string.
 * @return stringstream - Heap allocated instance.
 */
stringstream stringstream_alloc(const __uint32_t length);

/**
 * @brief Function allocates a stringstream instance using a const char*
 * instance.
 *
 * @param[in] string String to copy to stringstream instance.
 * @return stringstream - Heap allocated instance.
 */
stringstream stringstream_str_alloc(const char* string);

/**
 * @brief Function deallocates the memory occupied by the stringstream instance
 * in the free store.
 *
 * @param[in] sstream stringstream instance.
 */
void stringstream_dealloc(stringstream* sstream);

#endif