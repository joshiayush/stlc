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

#ifndef _CJSON_DATA_D_ARRAY_ITERATORS_H_
#define _CJSON_DATA_D_ARRAY_ITERATORS_H_

#include <sys/types.h>

#include "bool.h"
#include "data/d_array/d_array.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function executes the given predicate on each element of the @a
 * d_array instance.
 *
 * @param darray @a d_array instance.
 * @param pred Function to execute on each element of the @a d_array instance.
 */
void d_array_map(d_array *const darray, void (*pred)(const void *const elem));

/**
 * @brief Function to check if for any value in the @a d_array instance the
 * given predicate evaluates to true or not.
 *
 * @param darray @a d_array instance.
 * @param pred Function to execute on each element of the @a d_array instance.
 * @return bool_t @a TRUE if predicate returns @a TRUE for any value in @a
 * d_array instance.
 */
bool_t d_array_any(d_array *const darray,
                   bool_t (*pred)(const void *const elem));

/**
 * @brief Function to check if for all values in the @a d_array instance the
 * given predicate evaluates to true or not.
 *
 * @param darray @a d_array instance.
 * @param pred Function to execute on each element of the @a d_array instance.
 * @return bool_t @a TRUE if predicate returns @a TRUE for every value in @a
 * d_array instance.
 */
bool_t d_array_all(d_array *const darray,
                   bool_t (*pred)(const void *const elem));

#ifdef __cplusplus
}
#endif

#endif