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
 * @brief Function allocates a stringstream instance using the default string
 * size of SSTREAM_DEFAULT_SIZE.
 *
 * @return stringstream - Heap allocated instance.
 */
stringstream stringstream_alloc_default() {
  return stringstream_alloc(SSTREAM_DEFAULT_SIZE);
}

/**
 * @brief Function allocates a stringstream instance using the string size @p
 * length.
 *
 * @param[in] length Bytes to allocate for the string.
 * @return stringstream - Heap allocated instance.
 */
stringstream stringstream_alloc(const __uint32_t length) {
  stringstream sstream = {.data = (void*)0, .length = 0, .capacity = 0};

  __uint32_t capacity = 1;
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
  __uint32_t length = strlen(string);
  stringstream sstream = stringstream_alloc(length);
  if (sstream.capacity) {
    memcpy(sstream.data, string, length * sizeof(char));
    sstream.length = length;
  }
  return sstream;
}

/**
 * @brief Function deallocates the memory occupied by the stringstream instance
 * in the free store.
 *
 * @param[in] sstream stringstream instance.
 */
void stringstream_dealloc(stringstream* sstream) {
  sstream->length = 0;
  sstream->capacity = 0;
  free(sstream->data);
  sstream->data = (void*)0;
}