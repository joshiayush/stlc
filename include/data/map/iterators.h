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

#ifndef CJSON_INCLUDE_DATA_MAP_ITERATORS_H_
#define CJSON_INCLUDE_DATA_MAP_ITERATORS_H_

#include "data/map/map.h"

// Defines a ``function pointer`` type to take in a ``function pointer`` by
// reference.  Used in function
// ``void MapTraverse(Map *const, TraversePredicate)`` as its predicate.
typedef void (*TraversePredicate)(void *const key, void *const value);

// Defines a ``function pointer`` type to take in a ``function pointer`` by
// reference.  Used in function
// ``void MapTraverseWithMapInstance(Map *map,
//                                   TraverseWithMapInstancePredicate
//                                   predicate)``
// as its predicate.
typedef void (*TraverseWithMapInstancePredicate)(Map *const map,
                                                 void *const key,
                                                 void *const value);

// Traverses through the ``Map`` instance and executes the given ``predicate``
// on each pair of the ``Map`` instance.
//
// The given predicate must conform to the signature of the
// ``TraversePredicate`` type and must not try to update its pointers.
void MapTraverse(Map *const map, TraversePredicate predicate);

// Traverses through the ``Map`` instance and executes the given ``predicate``
// on each pair of the ``Map`` instance.
//
// This should be very reminiscent of what we did in function
// ``void MapTraverse(Map *const, TraversePredicate)`` except for the fact that
// this ``predicate`` also takes in a ``Map`` instance as its first parameter.
void MapTraverseWithMapInstance(Map *const map,
                                TraverseWithMapInstancePredicate predicate);

typedef struct MapIterator {
  Map *map;
  size_t cur_bucket_idx;
  MapEntry *cur_entry;
} MapIterator;

MapIterator MapIteratorNew(Map *const map);

MapEntry *MapIteratorNext(MapIterator *const it);

#endif  // CJSON_INCLUDE_DATA_MAP_ITERATORS_H_
