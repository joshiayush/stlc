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

#include "map/ops.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bool.h"
#include "map/map.h"

void MapInsert(Map *const map, const void *const key, const size_t key_size,
               const void *const value, const size_t value_size) {
  if (map == NULL || key == NULL || value == NULL) return;

  const hash_t hash = map->hash_func(key);
  const size_t bucket_index = hash % map->capacity;
  pthread_mutex_lock(&(map->mutex));

  MapEntry *entry = map->buckets[bucket_index];
  while (entry != NULL) {
    if (entry->hash == hash && map->key_eq_func(entry->key, key) == TRUE) {
      free(entry->value);
      entry->value = (void *)malloc(value_size);
      memcpy(entry->value, value, value_size);
      pthread_mutex_unlock(&(map->mutex));
      return;
    }
    entry = entry->next;
  }

  MapEntry *new_entry = (MapEntry *)malloc(sizeof(MapEntry));
  MapEntryInit(new_entry, key, key_size, value, value_size, hash,
               map->buckets[bucket_index]);
  map->buckets[bucket_index] = new_entry;
  ++(map->size);

  pthread_mutex_unlock(&(map->mutex));
}

void *MapGet(Map *const map, const void *key) {
  if (map == NULL || key == NULL) return NULL;

  hash_t hash = map->hash_func(key);
  size_t bucket_index = hash % map->capacity;

  MapEntry *entry = map->buckets[bucket_index];
  while (entry != NULL) {
    if (hash == entry->hash && map->key_eq_func(entry->key, key) == TRUE) {
      return entry->value;
    }
    entry = entry->value;
  }

  return NULL;
}

void MapRemove(Map *const map, const void *key, const size_t key_size) {
  if (map == NULL || key == NULL || MapGet(map, key) == NULL) return;

  const hash_t hash = map->hash_func(key);
  const size_t bucket_index = hash % map->capacity;

  pthread_mutex_lock(&(map->mutex));

  MapEntry *entry = map->buckets[bucket_index];
  MapEntry *prev_entry = NULL;
  while (entry != NULL) {
    if (entry->hash == hash && map->key_eq_func(entry->key, key) == TRUE) {
      if (prev_entry == NULL) {
        map->buckets[bucket_index] = entry->next;
      } else {
        prev_entry->next = entry->next;
      }
      free(entry->key);
      free(entry->value);
      free(entry);
      --(map->size);
      break;
    }
    prev_entry = entry;
    entry = entry->next;
  }

  pthread_mutex_unlock(&(map->mutex));
}
