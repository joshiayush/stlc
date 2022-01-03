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

#include "data/sstream/modifiers.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "data/sstream/sstream.h"

// Concatenates a newly formatted string onto the existing string inside
// ``StringStream`` instance and increments the ``length`` attribute.
void StringStreamConcat(StringStream* const sstream, const char* format, ...) {
  va_list args;
  va_start(args, format);
  size_t avail = _GET_STRING_STREAM_AVAILABLE_SPACE(*sstream);
  size_t format_size = vsnprintf(sstream->data + sstream->length,
                                 avail * sizeof(char), format, args);
  va_end(args);

  // ``vsnprintf()`` has overflow protection, so if this condition evaluates to
  // true that means ``vsnprintf()`` did not concatenate the new string
  // properly; this clause takes care of that.
  if (StringStreamRealloc(sstream, sstream->length + format_size) ==
      SSTREAM_REALLOC_SUCCESS) {
    va_start(args, format);
    avail = _GET_STRING_STREAM_AVAILABLE_SPACE(*sstream);
    format_size = vsnprintf(sstream->data + sstream->length,
                            avail * sizeof(char), format, args);
    va_end(args);
  }
  sstream->length += format_size;
  _TERMINATE_STRING_STREAM_BUFFER(*sstream);
}

// Concatenates data of known length onto the existing ``data`` instance of
// ``StringStream`` instance.  This function will re-allocate the ``data``
// buffer if needed.
//
// The ``length`` property will decide how many bytes to copy from ``data`` to
// ``sstream->data`` while ignoring the ``NULL`` bytes on the way.
void StringStreamRead(StringStream* const sstream, const void* data,
                      const size_t length) {
  (void)StringStreamRealloc(sstream, sstream->length + length);
  memcpy(sstream->data + sstream->length, data, length);
  sstream->length += length;
  _TERMINATE_STRING_STREAM_BUFFER(*sstream);
}

// Impedes the position of the terminate character ``\0`` by ``length``, or if
// the length is greater than the data length of ``StringStream`` instance,
// places the terminator ``\0`` at beginning.
void StringStreamRetreat(StringStream* const sstream, const size_t length) {
  if ((sstream != NULL) && (!sstream->length || !sstream->capacity))
    return;
  sstream->length = length >= sstream->length ? 0 : sstream->length - length;
  _TERMINATE_STRING_STREAM_BUFFER(*sstream);
}
