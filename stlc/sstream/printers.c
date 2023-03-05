// Copyright 2021, The cjson authors.
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
//     * Neither the name of The cjson authors. nor the names of its
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

#include "sstream/printers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bool.h"
#include "sstream/sstream.h"

// Escapes control characters and quotes in a string and appends it to the given
// buffer.
void ChrCStrLiteral(const char chr, char* buffer) {
  if ((chr >= 0x20 && chr <= 0x7E) && chr != '\'' && chr != '\"' &&
      chr != '\\') {  // Opposite of control characters can be saved just as
                      // they are.
    // `strcat` doesn't support character to string concatenation so we have to
    // make an extra step to convert our character to string.
    char chr_to_str_buffer[2] = {chr, '\0'};
    strcat(buffer, chr_to_str_buffer);
  } else {
    switch (chr) {
      case '\a':
        strcat(buffer, "\\a");
        break;
      case '\b':
        strcat(buffer, "\\b");
        break;
      case '\f':
        strcat(buffer, "\\f");
        break;
      case '\n':
        strcat(buffer, "\\n");
        break;
      case '\r':
        strcat(buffer, "\\r");
        break;
      case '\t':
        strcat(buffer, "\\t");
        break;
      case '\v':
        strcat(buffer, "\\v");
        break;
      case '\\':
        strcat(buffer, "\\\\");
        break;
      case '\'':
        strcat(buffer, "\\'");
        break;
      case '\"':
        strcat(buffer, "\\\"");
        break;
      default: {
        char buffer_[6];
        // This handles control characters that are not printable and does not
        // meet any case.
        snprintf(buffer_, 6, "\\0x%02x", (unsigned char)(chr));
        strcat(buffer, buffer_);
      } break;
    }
  }
}

// Returns a string with printable representation of escape sequences.
void ReprSstream(StringStream* sstream) {
  const size_t strlen_ = strlen(sstream->data);
  char* buffer = malloc(strlen_ * sizeof(char));
  for (size_t i = 0; i < strlen_; ++i) {
    ChrCStrLiteral(
        sstream->data[i],
        buffer);  // Escape the character that we took out in this iteration.
  }
  StringStreamDealloc(sstream);
  *sstream = StringStreamStrAlloc(buffer);
}

// Prints the given `StringStream` object to the `stdout` and flushes the stream
// quickly.
void PrintSstream(StringStream* sstream, const bool_t escape) {
  if (!escape) {
    printf("%s", sstream->data);
  } else {
    ReprSstream(sstream);
    printf("%s", sstream->data);
  }
  fflush(stdout);
}
