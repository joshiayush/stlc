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

#include "map/iterators.h"

#include <stdio.h>

#include "bool.h"
#include "map/iterators.h"
#include "map/map.h"

// Traverses the entire map and calls the given predicate function on each map
// element.
//
// Params:
//  map       - A pointer to the map to traverse.
//  predicate - A function pointer to the predicate function to call on each map
//              element.
//              The function should have the signature:
//                    void (*predicate)(void* key, void* value).
//              The key and value arguments passed to the function are pointers
//              to the actual key and value stored in the map.
//
// Returns:
//  The number of map elements traversed.
//
// Remarks:
//  The function acquires the map mutex lock before traversing the map to ensure
//  thread safety. The function does not modify the map or its elements.
void MapTraverse(Map *const map,
                 bool_t (*predicate)(const void *key, const void *value)) {
  if (map == NULL || predicate == NULL) return;

  pthread_mutex_lock(&map->mutex);

  for (size_t i = 0; i < map->capacity; i++) {
    MapEntry *entry = map->buckets[i];

    while (entry != NULL) {
      if (predicate(entry->key, entry->value) == FALSE) {
        pthread_mutex_unlock(&map->mutex);
        return;
      }
      entry = entry->next;
    }
  }

  pthread_mutex_unlock(&map->mutex);
}
