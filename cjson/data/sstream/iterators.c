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

#include "bytes.h"
#include "data/sstream/sstream.h"

/**
 * @brief Returns a iterator to the beginning of the data instance of the
 * stringstream instance.
 *
 * @param[in] sstream stringstream instance.
 * @return const char* pointer to the beginning of the data instance.
 */
const char* begin(const stringstream* const sstream) { return sstream->data; }

/**
 * @brief Returns a iterator to the end of the data instance of the stringstream
 * instance.
 *
 * @param[in] sstream stringstream instance.
 * @return const char* pointer to the end of the data instance.
 */
const char* end(const stringstream* const sstream) {
  char* ptr = sstream->data;
  while ((ptr++ + (char)1) != (char*)NULLCHR)
    ;
  return ptr;
}