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

#include "data/sstream/sstream.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "data/sstream/modifiers.h"

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
    const _stringstream_alloc_args sstream_var_args) {
  return _stringstream_alloc(sstream_var_args.length);
}

/**
 * @brief Function allocates a stringstream instance using the string size @p
 * length.
 *
 * @param[in] length Bytes to allocate for the string.
 * @return stringstream - Heap allocated instance.
 */
stringstream _stringstream_alloc(const size_t length) {
  stringstream sstream = {.data = (void*)0, .length = length, .capacity = 0};
  size_t capacity = 1;
  while (capacity < length)
    capacity <<= 1;
  if (sstream.data = malloc(capacity * sizeof(char))) {
    sstream.capacity = capacity;
    _terminate(sstream);
  }
  return sstream;
}

/**
 * @brief Function allocates a stringstream instance using a const char*
 * instance.
 *
 * @param[in] string String to copy to stringstream instance.
 * @return stringstream - Heap allocated instance.
 */
stringstream stringstream_str_alloc(const char* string) {
  size_t length = strlen(string);
  stringstream sstream = _stringstream_alloc(length);
  if (sstream.capacity) {
    memcpy(sstream.data, string, length * sizeof(char));
    sstream.length = length;
  }
  return sstream;
}

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
                               const size_t length) {
  if (length <= sstream->capacity)
    return REALLOC_NOT_REQUIRED;
  size_t capacity = sstream->capacity ? sstream->capacity : 1;
  while (capacity <= length + 1)
    capacity <<= 1;
  char* data = sstream->data;
  sstream->data = realloc(sstream->data, capacity * sizeof(char));
  if (!sstream->data) {
    if (!(sstream->data = malloc(capacity * sizeof(char)))) {
      sstream->data = data;
      return REALLOC_FAILURE;
    }
    memcpy(sstream->data, data, sstream->length * sizeof(char));
    free(data);
  }
  sstream->capacity = capacity;
  return REALLOC_SUCCESS;
}

/**
 * @brief Function deallocates the memory occupied by the stringstream instance
 * in the free store.
 *
 * @param[in] sstream stringstream instance.
 */
void stringstream_dealloc(stringstream* const sstream) {
  sstream->length = 0;
  sstream->capacity = 0;
  free(sstream->data);
  sstream->data = (void*)0;
}