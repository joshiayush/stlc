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

#ifndef _DATA_SSTREAM_MODIFIERS_H_
#define _DATA_SSTREAM_MODIFIERS_H_

#include "data/sstream/sstream.h"

#define NULL_CHAR '\0'

/**
 * @brief Terminates a string inside of the stringstream instance at '\0'.
 *
 * @param[in] sstream stringstream instance.
 *
 * @note This macro is private to the module sstream.
 */
#define _terminate(sstream)                         \
  do {                                              \
    if ((sstream).data && (sstream).capacity)       \
      (sstream).data[(sstream).length] = NULL_CHAR; \
  } while (0)

/**
 * @brief Concatenates a newly formatted string onto the existing string in
 * stringstream instance.
 *
 * @param sstream stringstream instance.
 * @param format Formatted string to concatenate.
 * @param ... Arguments to substitute the formatted string with.
 */
void stringstream_concat(stringstream* const sstream, const char* format, ...);

#endif