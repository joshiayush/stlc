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

#include "data/sstream/fileio.h"

#include <stdio.h>
#include <sys/types.h>

#include "data/sstream/modifiers.h"
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
                           size_t length) {
  if (length == 0) {
    size_t orig_cur_pos = ftell(file);
    fseek(file, 0L, SEEK_END);
    length = ftell(file) - orig_cur_pos;
    fseek(file, orig_cur_pos, SEEK_SET);
  }
  stringstream_realloc(sstream, sstream->length + length);
  fread(sstream->data + sstream->length, sizeof(char), length, file);
  sstream->length += length;
  _terminate(*sstream);
}

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
                            size_t begin, size_t end) {
  if (begin >= sstream->length)
    return;
  if (!end)
    end = sstream->length;
  if (end < begin)
    return;
  if (end > sstream->length)
    end = sstream->length;
  fwrite(sstream->data + begin, sizeof(char), end - begin, file);
}