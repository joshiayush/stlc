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

#ifndef _DATA_SSTREAM_ACCESSORS_H_
#define _DATA_SSTREAM_ACCESSORS_H_

/**
 * @brief Calculates the number of memory blocks available in the data instance
 * of the stringstream instance.
 *
 * @param[in] sstream stringstring instance.
 */
#define stringstream_avail(sstream) \
  (((sstream).capacity) - ((sstream).length) - 1)

#endif