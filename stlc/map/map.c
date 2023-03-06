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

// Initializes a MapEntry with the specified key, key size, value, value size,
// hash, and next MapEntry.
//
// Params:
//  map_entry  - A pointer to the MapEntry to be initialized.
//  key        - A pointer to the key.
//  key_size   - The size of the key.
//  value      - A pointer to the value.
//  value_size - The size of the value.
//  hash       - The hash value for the key.
//  next       - A pointer to the next MapEntry in the map.
void MapEntryInit(MapEntry* map_entry, const void* key, const size_t key_size,
                  const void* value, const size_t value_size, const hash_t hash,
                  MapEntry* const next) {
  map_entry->key = NULL;
  map_entry->value = NULL;
  if (map_entry == NULL || key == NULL || value == NULL) return;

  if ((map_entry->key = (void*)malloc(key_size)) == NULL) {
    fprintf(stderr, "MapEntryInit: failed to allocate key for key_size: %zu\n",
            key_size);
    return;
  }
  memcpy(map_entry->key, key, key_size);

  if ((map_entry->value = (void*)malloc(value_size)) == NULL) {
    fprintf(stderr,
            "MapEntryInit: failed to allocate value for value_size: %zu\n",
            value_size);
    return;
  }
  memcpy(map_entry->value, value, value_size);

  map_entry->hash = hash;
  map_entry->next = NULL;
}

// Initializes a new instance of the Map data structure with the specified
// capacity and hash and key comparison functions.
//
// Params:
//  map         - A pointer to the Map data structure to be initialized.
//  capacity    - The capacity of the Map, which is the number of buckets to
//                allocate.
//  hash_func   - A pointer to the hash function used to calculate hash codes
//                for keys.
//  key_eq_func - A pointer to the key comparison function used to compare keys
//                for equality.
//
// Remarks:
//  This function initializes a new instance of the Map data structure with the
//  specified capacity, hash function, and key comparison function. It sets the
//  size of the Map to 0 and allocates the necessary memory for the Map's
//  buckets. If the pointer passed to `map` is NULL, this function returns
//  immediately without doing anything.
//
//  The `hash_func` function should take a const void* pointer to a key and its
//  size as arguments and return a hash_t value. The `key_eq_func` function
//  should take two const void* pointers to keys and their sizes as arguments
//  and return a boolean value indicating whether they are equal or not.
void MapInit(Map* const map, const size_t capacity, hash_f hash_func,
             key_eq_f key_eq_func) {
  if (map == NULL) return;
  if (capacity < MAP_MIN_CAPACITY || capacity > MAP_MAX_CAPACITY) {
    fprintf(stderr, "MapInit: capacity out of range [%zu, %zu]: %zu\n",
            MAP_MIN_CAPACITY, MAP_MAX_CAPACITY, capacity);
    return;
  }

  map->capacity = capacity;
  map->size = 0;
  map->hash_func = hash_func;
  map->key_eq_func = key_eq_func;
  if ((map->buckets = (MapEntry**)calloc(capacity, sizeof(MapEntry*))) ==
      NULL) {
    fprintf(stderr, "MapInit: failed to allocate buckets for capacity: %zu\n",
            capacity);
    return;
  }
  if (pthread_mutex_init(&map->mutex, NULL) != 0) {
    fprintf(stderr, "MapInit: failed to initialize mutex\n");
    free(map->buckets);
  }
}

// Re-allocates a `Map` instance with the specified capacity inside the default
// capacity constraints, rehashing all the entries.
//
// Params:
//  map          - A pointer to the `Map` data structure to be re-allocated.
//  new_capacity - The new capacity of the `Map`, which is the number of buckets
//                 to allocate.
//
// Remarks:
//  This function acquires a mutex lock before accessing the map for thread
//  safety.
//    * If `map` is `NULL`, the function returns without doing anything.
//    * If allocation of `new_buckets` fails, the function returns with an error
//      message.
//
//  The function rehashes all entries in the current map to their new bucket
//  index in the new map
//    * If `map->size` is greater than `new_capacity`, `map->size` is set to
//      `new_capacity`.
void MapRealloc(Map* const map, const size_t new_capacity) {
  if (map == NULL) return;
  if (new_capacity < MAP_MIN_CAPACITY || new_capacity > MAP_MAX_CAPACITY) {
    fprintf(stderr, "MapRealloc: capacity out of range [%zu, %zu]: %zu\n",
            MAP_MIN_CAPACITY, MAP_MAX_CAPACITY, new_capacity);
    return;
  }

  pthread_mutex_lock(&map->mutex);

  MapEntry** new_buckets;
  if ((new_buckets = (MapEntry**)calloc(new_capacity, sizeof(MapEntry*))) ==
      NULL) {
    fprintf(stderr, "MapRealloc: failed to allocate buckets for capacity: %zu",
            new_capacity);
    return;
  }

  for (size_t i = 0; i < map->capacity; ++i) {
    MapEntry* entry = map->buckets[i];
    while (entry != NULL) {
      MapEntry* next_entry = entry->next;
      const size_t new_bucket_index = entry->hash % new_capacity;
      entry->next = new_buckets[new_bucket_index];
      new_buckets[new_bucket_index] = entry;
      entry = next_entry;
    }
  }
  free(map->buckets);
  map->buckets = new_buckets;
  map->capacity = new_capacity;
  if (map->size > map->capacity) map->size = map->capacity;

  pthread_mutex_unlock(&map->mutex);
}

// Frees up a `Map` instance and the entries associated with it.
//
// This function is resposible for clearning up the free-store occupied by your
// `Map` instance after calling this function the `Map` data reference passed
// becomes empty.
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
