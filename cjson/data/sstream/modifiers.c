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

#include "data/sstream/modifiers.h"

#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>

#include "data/sstream/accessors.h"
#include "data/sstream/sstream.h"

/**
 * @brief Concatenates a newly formatted string onto the existing string in
 * stringstream instance.
 *
 * @param sstream stringstream instance.
 * @param format Formatted string to concatenate.
 * @param ... Arguments to substitute the formatted string with.
 */
void stringstream_concat(stringstream* const sstream, const char* format, ...) {
  va_list args;
  va_start(args, format);
  size_t avail = stringstream_avail(*sstream);
  __int32_t format_size = vsnprintf(sstream->data + sstream->length,
                                    avail * sizeof(char), format, args);
  va_end(args);
  if (stringstream_realloc(sstream, sstream->length + format_size) ==
      REALLOC_SUCCESS) {  // vsnprintf() has overflow protection, so if this
                          // condition evaluates to true that means vsnprintf()
                          // did not concatenate the new string properly; this
                          // clause takes care of that
    va_start(args, format);
    avail = stringstream_avail(*sstream);
    format_size = vsnprintf(sstream->data + sstream->length,
                            avail * sizeof(char), format, args);
    va_end(args);
  }
  sstream->length += format_size;
  _terminate(*sstream);
}