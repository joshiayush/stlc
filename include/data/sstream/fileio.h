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

#ifndef CJSON_INCLUDE_DATA_SSTREAM_FILEIO_H_
#define CJSON_INCLUDE_DATA_SSTREAM_FILEIO_H_

#include <stdio.h>
#include <sys/types.h>

#include "data/sstream/sstream.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif  // CJSON_INCLUDE_DATA_SSTREAM_FILEIO_H_
