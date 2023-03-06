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

// Insert a new key-value pair into the map.
//
// Args:
//  map        - A pointer to the map to insert the key-value pair into.
//  key        - A pointer to the key to insert.
//  key_size   - The size of the key in bytes.
//  value      - A pointer to the value to insert.
//  value_size - The size of the value in bytes.
//
// Remarks:
//  If the map, key, or value pointers are NULL, this function will immediately
//  return without doing anything. The key and value pointers must point to
//  valid memory of the specified sizes. If a key already exists in the map, its
//  value will be replaced with the new value.
//
// Thread Safety:
//  This function locks the mutex associated with the map while it is performing
//  its operations to ensure thread safety.
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

// Retrieve the value associated with the given key in the map.
//
// Params:
//  map - A pointer to the map.
//  key - A pointer to the key.
//
// Returns:
//  A pointer to the value associated with the key, or NULL if the key is not
//  found in the map.
//
// Remarks:
//  This function assumes that the `hash_func` and `key_eq_func` fields of the
//  map are properly set to the hash function and key equality function used to
//  initialize the map. Also note that this function does not check if the map
//  or key pointers are NULL, as passing NULL to these parameters is considered
//  undefined behavior.
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

// Remove an entry from the map with the given key.
//
// Params:
//  map      - The map from which to remove the entry.
//  key      - The key of the entry to remove.
//  key_size - The size of the key in bytes.
//
// Effects:
//  * Removes an entry from the map with the given key, if it exists.
//  * Frees the memory used by the removed entry.
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
