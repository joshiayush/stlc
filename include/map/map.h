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

// We are pretty much doing everything in constant time if we don't have a load
// factor greater than `1`.
//
//                      #Entries
//      LoadFactor = ~~~~~~~~~~~~~~    <= 1 for O(1)
//                      #Buckets
#define MAX_LOAD_FACTOR 0.75f
#define MAP_DEFAULT_BUCKET_LEN (1 << 4)

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

void MapEntryInit(MapEntry* map_entry, const void* key, const size_t key_size,
                  const void* value, const size_t value_size, const hash_t hash,
                  MapEntry* const next);

typedef struct Map {
  hash_f hash_func;
  key_eq_f key_eq_func;
  MapEntry** buckets;
  size_t capacity;
  size_t size;
  pthread_mutex_t mutex;
} Map;

void MapInit(Map* const map, const size_t capacity, hash_f hash_func,
             key_eq_f key_eq_func);

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
