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

#ifndef CJSON_INCLUDE_DATA_MAP_MAP_H_
#define CJSON_INCLUDE_DATA_MAP_MAP_H_

#include <pthread.h>
#include <sys/types.h>

#include "bool.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAP_MIN_CAPACITY 0x20
#define MAP_MAX_CAPACITY 0xF4240

// Custom type to represent hash data.
typedef size_t hash_t;

// Function signature for the function defined to generate hash using the
// `key` value.
//
// Function defined with this signature takes in a `const void*` which means a
// value of any type can be placed in the `key` of the `Map` and using that
// `key` we can generate a `hash` which is a `hash_t` type value.
typedef hash_t (*hash_f)(const void* key);

// Function signature for the function defined to compare two different keys for
// their equality.
//
// Function defined with this signature takes `const void*` to the keys of the
// `Map` which means you can pass in a data of any type that makes a `key`
// pair inside the `Map`.
typedef bool_t (*key_eq_f)(const void* key1, const void* key2);

// Creates a Map entry inside of a bucket.  This map entry is later extended in
// case the `LoadFactor` exceeds by `1` due to collision.
typedef struct MapEntry {
  void* key;
  void* value;
  hash_t hash;

  // Pointer to the entires inside a bucket to combat collisions by keeping the
  // number of entries always less than the number of buckets:
  //
  //       +~~~~~~~~~~~~~~~~~~~~+      +~~~~~~~~~~~~~~~~~~~~+
  //       ! key|value|hash|next+~~~~> + key|value|hash|next+~~~> NULL
  //       +~~~~~~~~~~~~~~~~~~~~+      +~~~~~~~~~~~~~~~~~~~~+
  struct MapEntry* next;
} MapEntry;

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
                  MapEntry* const next);

// The Map structure represents a hash table that associates keys with values.
// It contains the following fields:
//
// Attributes:
//  hash_func   - a function pointer to the hash function used to generate hash
//                values for keys.
//  key_eq_func - a function pointer to the key equality function used to
//                compare keys for equality.
//  buckets     - a pointer to an array of MapEntry pointers, which represent
//                the entries stored in the hash table.
//  capacity    - the maximum number of MapEntry pointers that can be stored in
//                buckets.
//  size        - the number of MapEntry pointers currently stored in buckets.
//  mutex       - a mutex used to synchronize access to the hash table in a
//                multi-threaded context.
typedef struct Map {
  hash_f hash_func;
  key_eq_f key_eq_func;
  MapEntry** buckets;
  size_t capacity;
  size_t size;
  pthread_mutex_t mutex;
} Map;

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
             key_eq_f key_eq_func);

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
void MapRealloc(Map* const map, const size_t new_capacity);

// Frees up a `Map` instance and the entries associated with it.
//
// This function is resposible for clearning up the free-store occupied by your
// `Map` instance after calling this function the `Map` data reference passed
// becomes empty.
void MapFree(Map* const map);

// Creates a hash from a `key` of `string` data type.
//
// This functionality allow us to place a `key` inside our `Map` provided that
// the given `key` is a `string` data type.  We read `keylen` bytes from the
// given `key` and accumulate a `hash` value.
hash_t Hash(const void* const key);

// Compares the eqaulity of two `keys` of `string` data type.
//
// We compare `key1` with `key2` to create a result.  Key should be of `string`
// data type and must have a `NULL` terminator character.
bool_t KeyCmp(const void* key1, const void* key2);

#ifdef __cplusplus
}
#endif

#include "map/iterators.h"
#include "map/ops.h"

#endif  // CJSON_INCLUDE_DATA_MAP_MAP_H_
