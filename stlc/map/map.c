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

#include "map/map.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "map/ops.h"

// Creates a hash from a `key` of `string` data type.
//
// This functionality allow us to place a `key` inside our `Map` provided that
// the given `key` is a `string` data type. We read `keylen` bytes from the
// given `key` and accumulate a `hash` value.
hash_t Hash(const void* const key) {
  hash_t hash = 0X1505;
  if (key == NULL) return hash;

  const unsigned char* key_ = (const unsigned char*)key;
  size_t keylen = strlen((const char*)key_);

  // This implementation uses the FNV-1a algorithm, which is a simple but
  // effective hash function for strings. It starts with an initial value of
  // 5381 and multiplies it by 33 (left shift by 5 and then add) for each
  // character in the string. Finally, it adds the character value to the hash.
  // The hash value is returned at the end.
  for (size_t i = 0; i < keylen; ++i) {
    hash = ((hash << 0X5) + hash) + key_[i];
  }
  return hash;
}

// Compares the eqaulity of two `keys` of `string` data type.
//
// We compare `key1` with `key2` to create a result.  Key should be of `string`
// data type and must have a `NULL` terminator character.
bool_t KeyCmp(const void* key1, const void* key2) {
  if (key1 == NULL && key2 == NULL) return TRUE;
  if (key1 == NULL || key2 == NULL) return FALSE;
  return strcmp((char*)key1, (char*)key2) == 0 ? TRUE : FALSE;
}

void MapEntryInit(MapEntry* map_entry, const void* key, const size_t key_size,
                  const void* value, const size_t value_size, const hash_t hash,
                  MapEntry* const next) {
  map_entry->key = NULL;
  map_entry->value = NULL;
  if (map_entry == NULL || key == NULL || value == NULL) return;

  map_entry->key = (void*)malloc(key_size);
  assert(map_entry->key != NULL);
  memcpy(map_entry->key, key, key_size);

  map_entry->value = (void*)malloc(value_size);
  assert(map_entry->value != NULL);
  memcpy(map_entry->value, value, value_size);

  map_entry->hash = hash;
  map_entry->next = NULL;
}

void MapInit(Map* const map, const size_t capacity, hash_f hash_func,
             key_eq_f key_eq_func) {
  if (map == NULL) return;
  map->capacity = capacity;
  map->size = 0;
  map->hash_func = hash_func;
  map->key_eq_func = key_eq_func;
  map->buckets = (MapEntry**)calloc(capacity, sizeof(MapEntry*));
  map->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
}

void MapFree(Map* map) {
  if (map == NULL) return;

  for (size_t i = 0; i < map->capacity; ++i) {
    MapEntry* entry = map->buckets[i];
    while (entry != NULL) {
      MapEntry* next_entry = entry->next;
      free(entry->key);
      free(entry->value);
      free(entry);
      entry = next_entry;
    }
  }

  free(map->buckets);
  pthread_mutex_destroy(&map->mutex);
}
