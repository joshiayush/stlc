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

// Creates a `MapEntry` instance with the given key-value pairs and a hash.
//
// Allocates a `MapEntry` instance in the free-store and fills that memory
// with the given values.  Remember to free the returned `MapEntry` instance
// when not needed.
MapEntry* MapAllocEntryWithHash(const void* key, const void* value,
                                const hash_t hash) {
  MapEntry* mapentry = (MapEntry*)malloc(sizeof(MapEntry));

  mapentry->key = malloc(strlen(key) * sizeof(char));
  assert(mapentry->key != NULL);
  strcpy(mapentry->key, key);

  mapentry->value = malloc(strlen(value) * sizeof(char));
  assert(mapentry->value != NULL);
  strcpy(mapentry->value, value);

  mapentry->hash = hash;
  mapentry->next = NULL;
  return mapentry;
}

// Allocates a `Map` instance of a default bucket length of
// `MAP_DEFAULT_BUCKET_LEN` provided the `hash` function to generate a hash and
// a `keycmp` function to compare two distinct keys inside a `Map` instance is
// given.
//
// While allocating a `Map` instance you can also use the built-in hash
// generator function `Hash()` to generate a hash value from a `key` regardless
// of its data type and also the built-in key compare function `KeyCmp()` to
// compare two distinct keys.
Map MapAlloc(hash_f hash, keycmp_f keycmp) {
  return MapAllocNBuckets(MAP_DEFAULT_BUCKET_LEN, hash, keycmp);
}

// Allocates a `Map` instance of the given `bucketslen` provided the `hash`
// function to generate a hash and a `keycmp` function to compare two distinct
// keys inside a `Map` instance.
//
// While allocating a `Map` instance you can also use the built-in hash
// generator function `Hash()` to generate a hash value from a `key` regardless
// of its data type and also the built-in key compare function `KeyCmp()` to
// compare two distinct keys.
Map MapAllocNBuckets(size_t bucketslen, hash_f hash, keycmp_f keycmp) {
  if (bucketslen == 0)
    bucketslen = MAP_DEFAULT_BUCKET_LEN;
  if (hash == NULL)
    hash = Hash;
  if (keycmp == NULL)
    keycmp = KeyCmp;
  // clang-format off
  Map map = {.bucketslen = bucketslen, .entrieslen = 0,
             .buckets = (void*)0, .hash = hash, .keycmp = keycmp};
  // clang-format on
  if ((map.buckets = (MapEntry**)(malloc(bucketslen * sizeof(MapEntry*)))) ==
      NULL)
    return map;
  for (size_t i = 0; i < map.bucketslen; ++i)
    *(map.buckets + i) = NULL;
  return map;
}

// Allocates a `Map` instance when the number of entries that are going to be in
// our bucket is given.
//
// Explicitly providing the number of entries in the entire `Map` instance is
// helpful to keep the `MAX_LOAD_FACTOR` below 0.75 by increasing the amount of
// buckets our `Map` instance can hold, thus minimizing the chances of
// collision.
//
// While allocating a `Map` instance you can also use the built-in hash
// generator function `Hash()` to generate a hash value from a `key` regardless
// of its data type and also the built-in key compare function `KeyCmp()` to
// compare two distinct keys.
Map MapAllocNEntries(const size_t entrieslen, hash_f hash, keycmp_f keycmp) {
  double bucketslen = MAP_DEFAULT_BUCKET_LEN;
  while (((double)entrieslen / bucketslen) > MAX_LOAD_FACTOR)
    bucketslen *= 2.0;
  return MapAllocNBuckets((size_t)bucketslen, hash, keycmp);
}

// Re-allocates a `Map` instance by extending the `bucketslen` until the
// following does not evaluates to true:
//
//                      #Entries
//      LoadFactor > ~~~~~~~~~~~~~~
//                      #Buckets
//
// Calling this function will result in a new `Map` instance which would be
// the exact copy of the previous `Map` instance except for the `bucketslen`.
//
// We want to re-allocate the `Map` instance and extend the number of buckets
// it currently has so to combat the chances of collisions.
void MapRealloc(Map* map) {
  Map map_ = MapAllocNEntries(map->entrieslen, map->hash, map->keycmp);

  const size_t bucketslen = map_.bucketslen;
  for (size_t i = 0; i < map_.bucketslen; ++i) {
    MapEntry* current = *(map->buckets + i);
    while (current) {
      MapPut(&map_, current->key, current->value);
      current = current->next;
    }
  }

  MapEntry** tmp_buckets = map->buckets;
  map->buckets = map_.buckets;
  map_.buckets = tmp_buckets;
  map_.bucketslen = map->bucketslen;
  MapFree(&map_);
  map->bucketslen = bucketslen;
}

// Copies `src` to `dest`.
//
// This function will not make the copies of the values stored inside of the
// `src` hash map but will create a new bucket list of pointers pointing to the
// values inside `src` hash map.
//
// This is mainly used when `src` instance is stored in the `stack` while the
// values inside of it are stored in the `free-store` and you don't want to lose
// the memory when `src` goes out of scope; thus it's better to copy the entire
// `src` hash map bucket into a new bucket that is dynamically allocated.
void MapCopy(Map* const dest, Map* const src) {
  if (src == NULL) {
    dest->buckets = NULL;
    dest->bucketslen = MAP_DEFAULT_BUCKET_LEN;
    dest->entrieslen = 0;
    dest->hash = NULL;
    dest->keycmp = NULL;
    return;
  }
  if (src->entrieslen > dest->entrieslen) {
    dest->entrieslen = src->entrieslen;
    MapRealloc(dest);
  }
  for (size_t i = 0; i < src->bucketslen; ++i)
    *(dest->buckets + i) = *(src->buckets + i);
}

// Frees up a `Map` instance and the entries associated with it.
//
// This function is resposible for clearning up the free-store occupied by your
// `Map` instance after calling this function the `Map` data reference passed
// becomes empty.
void MapFree(Map* const map) {
  for (size_t i = 0; i < map->bucketslen; ++i) {
    if (*(map->buckets + i)) {
      MapFreeEntryImpl(*(map->buckets + i));
      free(*(map->buckets + i));
    }
  }
  map->bucketslen = 0;
  map->entrieslen = 0;
  free(map->buckets);
}

// Frees up a `Map` instance and the entries associated with it with their
// `key-value` pairs.
//
// This function is resposible for clearning up the free-store occupied by your
// `Map` instance after calling this function the `Map` data reference passed
// becomes empty and the values stored as `key-value` pairs inside of the `Map`
// instance will be destroyed forever.
void MapFreeDeep(Map* const map) {
  for (size_t i = 0; i < map->bucketslen; ++i) {
    if (map->buckets[i]) {
      MapFreeEntryDeepImpl(*(map->buckets + i));
      free(*(map->buckets + i));
    }
  }
  map->bucketslen = 0;
  map->entrieslen = 0;
  free(map->buckets);
}

// Private function to free up the space occupied by the entries in a bucket.
//
// Takes in a `MapEntry` instance but frees up all the other entries linked
// with that map entry and leaves the map entry given intially unchanged.
//
// Only the neighbours are freed using this function not the head node you still
// need to free the head node by yourself.  This function is an implementation
// detail, you should not use it as it is not the part of the public API.
void MapFreeEntryImpl(MapEntry* const mapentry) {
  if (mapentry == NULL)
    return;
  if (mapentry->next) {
    MapFreeEntryImpl(mapentry->next);
    free(mapentry->next);
  }
}

// Private function to free up the space occupied by the entries and their
// respective `key-value` pairs in a bucket.
//
// Takes in a `MapEntry` instance but frees up all the other entries linked
// with that map entry and leaves the map entry given intially unchanged.
//
// Only the neighbours are freed using this function not the head node you still
// need to free the head node by yourself.  This function is an implementation
// detail, you should not use it as it is not the part of the public API.
//
// The `key-value` pair stored inside of the initial `MapEntry` will always get
// destroyed from the free-store but not the `MapEntry` container.
void MapFreeEntryDeepImpl(MapEntry* const mapentry) {
  if (mapentry) {
    if (mapentry->next) {
      MapFreeEntryDeepImpl(mapentry->next);
      free(mapentry->next);
    }
    free(mapentry->key);
    free(mapentry->value);
  }
}

// Returns a `Map` instance with the built-in support for hash generation and
// key comparison.  Key must always be a string data and the value could be
// anything.
Map MapAllocStrAsKey() { return MapAlloc(Hash, KeyCmp); }

// Returns a `Map` instance with the number of given `entrieslen` entries and
// with the built-in support for hash generation and key comparison.
Map MapAllocNStrAsKey(const size_t entrieslen) {
  return MapAllocNEntries(entrieslen, Hash, KeyCmp);
}

// Creates a hash from a `key` of `string` data type.
//
// This functionality allow us to place a `key` inside our `Map` provided that
// the given `key` is a `string` data type.  We read `keylen` bytes from the
// given `key` and accumulate a `hash` value.
hash_t Hash(const void* const key) {
  hash_t hash = 0;
  if (key == NULL)
    return hash;
  const char* key_ = (char*)key;
  size_t keylen = strlen(key_);
  for (size_t i = 0; i < keylen; ++i)
    hash = (((hash_t)(*(key_ + i))) + (0x1F * hash));
  return hash;
}

// Compares the eqaulity of two `keys` of `string` data type.
//
// We compare `key1` with `key2` to create a result.  Key should be of `string`
// data type and must have a `NULL` terminator character.
bool_t KeyCmp(const void* key1, const void* key2) {
  if (key1 == NULL && key2 == NULL)
    return TRUE;
  if (key1 == NULL || key2 == NULL)
    return FALSE;
  return strcmp((char*)key1, (char*)key2) == 0 ? TRUE : FALSE;
}
