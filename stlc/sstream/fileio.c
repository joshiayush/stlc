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

#include "sstream/fileio.h"

#include <stdio.h>
#include <sys/types.h>

#include "sstream/sstream.h"

// Reads the content of the file given of the given `length` in the
// `StringStream` instance.
//
// The length of the data read from the file by this function is equal to the
// value of the `length` given but in case the `length` given is `0` then this
// function will read the entire file.
void StringStreamReadFile(StringStream* const sstream, FILE* const file,
                          size_t length) {
  if (sstream == NULL || file == NULL)
    return;
  if (length == 0) {
    size_t orig_cur_pos = ftell(file);
    fseek(file, 0L, SEEK_END);
    length = ftell(file) - orig_cur_pos;
    fseek(file, orig_cur_pos, SEEK_SET);
  }
  StringStreamRealloc(sstream, sstream->length + length);
  fread(sstream->data + sstream->length, sizeof(char), length, file);
  sstream->length += length;
  _TERMINATE_STRING_STREAM_BUFFER(*sstream);
}

// Writes `StringStream` data from position `begin` to `end` in the given file.
void StringStreamWriteFile(StringStream* const sstream, FILE* const file,
                           size_t begin, size_t end) {
  if (sstream == NULL || file == NULL)
    return;
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
