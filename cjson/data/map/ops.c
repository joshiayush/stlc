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
#include <stdlib.h>

#include "bool.h"
#include "data/map/map.h"

// Calculates the index of element in the map.
//
// Uses `key hash` and the `length` of the bucket to determine the hash value
// for the element in the map.
size_t CalculateIndex(hash_t hash, size_t n) { return hash & (n - 0x01); }

// Injects the given set of key-value pair to the given `Map` instance if
// already exists, overrides it.
//
// Generates a hash value using the given key and finds the position of a new
// `MapEntry` instance in the `buckets` linked list.  To combat collision we
// extend the already saved `MapEntry` by hooking it up with the new `MapEntry`
// instance while keeping the order sorted based on the hash computed.
//
// Resizes the `Map` instance in case the load factor exceeds the
// `MAX_LOAD_FACTOR`.
void MapPut(Map *map, void *const key, void *const value) {
  hash_t hash = map->hash(key);
  size_t idx = CalculateIndex(hash, map->bucketslen);

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

// Returns a `void*` to the value mapped by the given `key`.
//
// Traverse through the entries of the bucket at the computed `idx` value and
// searches for a `hash` value that matches the computed `hash` value.  If we
// find a value that is greater than the `hash` computed; not equal but greater
// than then we immediately return `NULL` as having a value greater than the
// computed `hash` using the `key` can only be possible when we have traversed
// long enough but did not find any `hash` value equals to the computed `hash`.
//
// This should be very reminiscent of what we are doing in function
// `MapEntry *MapGetEntry(Map *const map, void *const key)`.
void *MapGet(Map *const map, void *const key) {
  MapEntry *mapentry = MapGetEntry(map, key);
  return mapentry ? mapentry->value : NULL;
}

// Returns a `MapEntry*` to the `MapEntry` instance that holds the given `key`.
//
// Traverse through the entries of the bucket at the computed `idx` value and
// searches for a `hash` value that matches the computed `hash` value.  If we
// find a value that is greater than the `hash` computed; not equal but greater
// than then we immediately return `NULL` as having a value greater than the
// computed `hash` using the `key` can only be possible when we have traversed
// long enough but did not find any `hash` value equals to the computed `hash`.
MapEntry *MapGetEntry(Map *const map, void *const key) {
  hash_t hash = map->hash(key);
  size_t idx = CalculateIndex(hash, map->bucketslen);

  if (map->buckets[idx] == NULL)
    return NULL;

  MapEntry *current = map->buckets[idx];

  while (current) {
    if (hash < current->hash)
      return (current = NULL);
    if (hash == current->hash && map->keycmp(key, current->key))
      break;
    current = current->next;
  }

  return current;
}

// Returns a `void*` and removes to/the value mapped by the given `key`.
//
// `MapEntry` with the associated key i.e., `key` will be deleted from the
// free-store while returning a `void*` to the value mapped with the given
// `key`.
//
// This should be very reminiscent of what we are doing in function `MapEntry
// *MapGetEntry(Map *const map, void *const key)` except after locating the
// entry in the `buckets` list we free the memory occupied by that `MapEntry`.
// This function is not responsible to free up the free-store occupied by the
// value inside of the `MapEntry` the caller should take care of that.
void *MapRemove(Map *const map, void *const key) {
  hash_t hash = map->hash(key);
  size_t idx = CalculateIndex(hash, map->bucketslen);

  if (map->buckets[idx] == NULL)
    return NULL;

  MapEntry *current = map->buckets[idx];
  MapEntry *prev = NULL;

  while (current) {
    if (hash < current->hash)
      return (current = NULL);
    if (hash == current->hash && map->keycmp(key, current->key))
      break;
    prev = current;
    current = current->next;
  }

  if (current == NULL)
    return NULL;

  void *ret = current->value;
  if (prev)
    prev->next = current->next;
  else
    map->buckets[idx] = current->next;
  free(current);
  --(map->entrieslen);

  return ret;
}
