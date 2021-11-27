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

#ifndef _DATA_SSTREAM_FILEIO_H_
#define _DATA_SSTREAM_FILEIO_H_

#include <stdio.h>
#include <sys/types.h>

#include "data/sstream/sstream.h"

/**
 * @brief Reads the content of the file given in the stringstream instance.
 *
 * @param[in] sstream stringstream instance.
 * @param[in] file File pointer.
 * @param[in] length Length of the data to be read. Reads the entire file if the
 * length given is 0.
 */
void stringstream_readfile(stringstream* const sstream, FILE* const file,
                           size_t length);

/**
 * @brief Writes stringstream data to the file given.
 *
 * @param[in] sstream stringstream instance to write.
 * @param[in] file File to write the stringstream instance data in.
 * @param[in] begin stringstream data element position from where to start
 * writing to the file.
 * @param[in] end stringstream data element position where to end writing to the
 * file.
 */
void stringstream_writefile(stringstream* const sstream, FILE* const file,
                            size_t begin, size_t end);

#endif