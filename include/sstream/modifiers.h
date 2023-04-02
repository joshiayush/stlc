// Copyright 2021, The stlc authors.
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
//     * Neither the name of The stlc authors. nor the names of its
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

#ifndef STLC_INCLUDE_DATA_SSTREAM_MODIFIERS_H_
#define STLC_INCLUDE_DATA_SSTREAM_MODIFIERS_H_

#include <sys/types.h>

#include "bytes.h"
#include "sstream/sstream.h"

#ifdef __cplusplus
extern "C" {
#endif

// Terminates a `StringStream` `data` instance at `\0`.
//
// This macro is meant to be protected inside `sstream` module.
#define _TERMINATE_STRING_STREAM_BUFFER(sstream)  \
  do {                                            \
    if ((sstream).data && (sstream).capacity)     \
      (sstream).data[(sstream).length] = nullchr; \
  } while (0)

// Concatenates a newly formatted string onto the existing string inside
// `StringStream` instance and increments the `length` attribute.
void StringStreamConcat(StringStream* const sstream, const char* format, ...);

// Concatenates data of known length onto the existing `data` instance of
// `StringStream` instance.  This function will re-allocate the `data` buffer if
// needed.
//
// The `length` property will decide how many bytes to copy from `data` to
// `sstream->data` while ignoring the `NULL` bytes on the way.
void StringStreamRead(StringStream* const sstream, const void* data,
                      const size_t length);

// Impedes the position of the terminate character `\0` by `length`, or if the
// length is greater than the data length of `StringStream` instance, places the
// terminator `\0` at beginning.
void StringStreamRetreat(StringStream* const sstream, const size_t length);

#ifdef __cplusplus
}
#endif

#endif  // STLC_INCLUDE_DATA_SSTREAM_MODIFIERS_H_
