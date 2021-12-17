// Copyright 2021, joshiayush Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of joshiayush Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef CJSON_INCLUDE_DATA_D_ARRAY_ACCESSORS_H_
#define CJSON_INCLUDE_DATA_D_ARRAY_ACCESSORS_H_

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

#endif  // CJSON_INCLUDE_DATA_D_ARRAY_ACCESSORS_H_
