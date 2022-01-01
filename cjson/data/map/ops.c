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

#include "data/map/ops.h"

#include <stdio.h>

#include "bool.h"
#include "data/map/map.h"

// Injects the given set of key-value pair to the given 'Map' instance if
// already exists, overrides it.
//
// Generates a hash value using the given key and finds the position of a new
// 'MapEntry' instance in the 'buckets' linked list.  To combat collision we
// extend the already saved 'MapEntry' by hooking it up with the new 'MapEntry'
// instance while keeping the order sorted based on the hash computed.
//
// Resizes the 'Map' instance in case the load factor exceeds the
// 'MAX_LOAD_FACTOR'.
void MapPut(Map *map, void *const key, void *const value) {
  hash_t hash = map->hash(key);
  size_t idx = hash % map->bucketslen;

  MapEntry *mapentry = MapAllocEntryWithHash(key, value, hash);

  if (*(map->buckets + idx) == NULL) {
    *(map->buckets + idx) = mapentry;
    ++(map->entrieslen);
  } else {
    MapEntry *current = map->buckets[idx];
    while (current->next && hash > current->hash)
      current = current->next;
    if ((hash == current->hash) && map->keycmp(key, current->key) == TRUE) {
      current->value = value;
    } else {
      MapEntry *tmp_mapentry = current->next;
      current->next = mapentry;
      mapentry->next = tmp_mapentry;
      ++(map->entrieslen);
    }
  }

  if (((double)map->entrieslen / (double)map->bucketslen) > MAX_LOAD_FACTOR)
    MapRealloc(map);
}
