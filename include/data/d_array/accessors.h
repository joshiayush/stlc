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

#ifndef _CJSON_DATA_D_ARRAY_ACCESSORS_H_
#define _CJSON_DATA_D_ARRAY_ACCESSORS_H_

#include <sys/types.h>

#include "data/d_array/d_array.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Sets the value in a @a d_array instance provided that the @p idx value
 * given is in bounds.
 *
 * @param[in] darray @a d_array instance.
 * @param[in] elem Element to add or to replace the @a d_array data with.
 * @param[in] idx Index where to add the @p elem given.
 */
void d_array_set(d_array* const darray, const void* const elem,
                 const size_t idx);

/**
 * @brief Returns the @a d_array instance data at given @p idx.
 *
 * @param[in] darray @a d_array instance.
 * @param[in] idx Element index.
 * @return const void* to the data located at the location.
 */
const void* d_array_get(const d_array* const darray, const size_t idx);

#ifdef __cplusplus
}
#endif

#endif