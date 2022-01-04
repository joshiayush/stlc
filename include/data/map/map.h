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

#ifndef CJSON_INCLUDE_DATA_MAP_MAP_H_
#define CJSON_INCLUDE_DATA_MAP_MAP_H_

#include <sys/types.h>

#include "bool.h"

// We are pretty much doing everything in constant time if we don't have a load
// factor greater than ``1``.
//
//                      #Entries
//      LoadFactor = ~~~~~~~~~~~~~~    <= 1 for O(1)
//                      #Buckets
#define MAX_LOAD_FACTOR 0.75f
#define MAP_DEFAULT_BUCKET_LEN (1 << 4)

#ifdef __cplusplus
extern "C" {
#endif

// Custom type to represent hash data.
typedef size_t hash_t;

// Function signature for the function defined to generate hash using the
// ``key`` value.
//
// Function defined with this signature takes in a ``const void*`` which means a
// value of any type can be placed in the ``key`` of the ``Map`` and using that
// ``key`` we can generate a ``hash`` which is a ``hash_t`` type value.
typedef hash_t (*hash_f)(const void* key);

// Function signature for the function defined to compare two different keys for
// their equality.
//
// Function defined with this signature takes ``const void*`` to the keys of the
// ``Map`` which means you can pass in a data of any type that makes a ``key``
// pair inside the ``Map``.
typedef bool_t (*keycmp_f)(const void* key1, const void* key2);

// Creates a Map entry inside of a bucket.  This map entry is later extended in
// case the ``LoadFactor`` exceeds by ``1`` due to collision.
typedef struct MapEntry {
  void* key;
  void* value;
  hash_t hash;
  // Pointer to the entires inside a bucket to combat collisions.
  //
  // +~~~~~~~~~~~~~~~~+  +~> +~~~~~~~~~~~~~~~~+
  // !      key       !  !   !      key       !
  // !      value     !  !   !      value     !
  // !      hash      !  !   !      hash      !
  // +~~~~~~~~~~~~~~~~+  !   +~~~~~~~~~~~~~~~~+
  // !      next  ~~~~+~~+   !      next  ~~~~+~~> NULL
  // +~~~~~~~~~~~~~~~~+      +~~~~~~~~~~~~~~~~+
  struct MapEntry* next;
} MapEntry;

// Creates a ``MapEntry`` instance with the given key-value pairs and a hash.
//
// Allocates a ``MapEntry`` instance in the free-store and fills that memory
// with the given values.  Remember to free the returned ``MapEntry`` instance
// when not needed.
MapEntry* MapAllocEntryWithHash(void* key, void* value, const hash_t hash);

typedef struct Map {
  hash_f hash;
  keycmp_f keycmp;

  // Each bucket is an element of a singly linked list so we want to have access
  // to its head.
  MapEntry** buckets;

  size_t bucketslen;
  size_t entrieslen;
} Map;

// Allocates a ``Map`` instance of a default bucket length of
// ``MAP_DEFAULT_BUCKET_LEN`` provided that the ``hash`` function to generate a
// hash and a ``keycmp`` function to compare two distinct keys inside a ``Map``
// instance is given.
//
// While allocating a ``Map`` instance you can also use the built-in hash
// generator function ``Hash()`` to generate a hash value from a ``key``
// regardless of its data type and also the built-in key compare function
// ``KeyCmp()`` to compare two distinct keys regardless of their data type but
// both of the keys must have the same data type and the size occupied by the
// type in the memory must be explicitly provided.
Map MapAlloc(hash_f hash, keycmp_f keycmp);

// Allocates a ``Map`` instance of the given ``bucketslen`` provided that the
// ``hash`` function to generate a hash and a ``keycmp`` function to compare two
// distinct keys inside a ``Map`` instance is given.
//
// While allocating a ``Map`` instance you can also use the built-in hash
// generator function ``Hash()`` to generate a hash value from a ``key``
// regardless of its data type and also the built-in key compare function
// ``KeyCmp()`` to compare two distinct keys regardless of their data type but
// both of the keys must have the same data type and the size occupied by the
// type in the memory must be explicitly provided.
Map MapAllocNBuckets(size_t bucketslen, hash_f hash, keycmp_f keycmp);

// Allocates a ``Map`` instance when the number of entries that are going to be
// in our bucket is given.
//
// Explicitly providing the number of entries in the entire ``Map`` instance is
// helpful to keep the ``MAX_LOAD_FACTOR`` below 0.75 by increasing the amount
// of buckets our ``Map`` instance can hold thus minimizing the chances of
// collision.
//
// While allocating a ``Map`` instance you can also use the built-in hash
// generator function ``Hash()`` to generate a hash value from a ``key``
// regardless of its data type and also the built-in key compare function
// ``KeyCmp()`` to compare two distinct keys regardless of their data type but
// both of the keys must have the same data type and the size occupied by the
// type in the memory must be explicitly provided.
Map MapAllocNEntries(const size_t entrieslen, hash_f hash, keycmp_f keycmp);

// Reallocates a ``Map`` instance by extending the ``bucketslen`` until the
// following does not evaluates to true:
//
//                      #Entries
//      LoadFactor > ~~~~~~~~~~~~~~
//                      #Buckets
//
// Calling this function will result in a new ``Map`` instance which would be
// the exact copy of the previous ``Map`` instance except for the
// ``bucketslen``.
//
// We want to re-allocate the ``Map`` instance and extend the number of buckets
// it currently has so to combat the chances of collisions.
void MapRealloc(Map* map);

// Copies ``src`` to ``dest``.
//
// This function will not make the copies of the values stored inside of the
// ``src`` hash map but will create a new bucket list of pointers pointing to
// the values inside ``src`` hash map.
//
// This is mainly used when ``src`` instance is stored in the ``stack`` while
// the values inside of it are stored in the ``free-store`` and you don't want
// to lose the memory when ``src`` goes out of scope; thus it's better to copy
// the entire ``src`` hash map bucket into a new bucket that is dynamically
// allocated.
void MapCopy(Map* const dest, Map* const src);

// Frees up a ``Map`` instance and the entries associated with it.
//
// This function is resposible for clearning up the free-store occupied by your
// ``Map`` instance after calling this function the ``Map`` data reference
// passed becomes empty.
void MapFree(Map* const map);

// Frees up a ``Map`` instance and the entries associated with it with their
// ``key-value`` pairs.
//
// This function is resposible for clearning up the free-store occupied by your
// ``Map`` instance after calling this function the ``Map`` data reference
// passed becomes empty and the values stored as ``key-value`` pairs inside of
// the ``Map`` instance will be destroyed forever.  So there is no way back to
// the ``key-value`` pairs once you call this function.
void MapFreeDeep(Map* const map);

// Private function to free up the space occupied by the entries in a bucket.
//
// Takes in a ``MapEntry`` instance but frees up all the other entries linked
// with that map entry and leaves the map entry given intially unchanged.
//
// Only the neighbours are freed using this function not the head node you still
// need to free the head node by yourself.  This function is an implementation
// detail you should not use it as it is not the part of the public API.
void _MapFreeEntry(MapEntry* const mapentry);

// Private function to free up the space occupied by the entries and their
// respective ``key-value`` pairs in a bucket.
//
// Takes in a ``MapEntry`` instance but frees up all the other entries linked
// with that map entry and leaves the map entry given intially unchanged.
//
// Only the neighbours are freed using this function not the head node you still
// need to free the head node by yourself.  This function is an implementation
// detail you should not use it as it is not the part of the public API.
//
// Although the ``key-value`` pair stored inside of the initial ``MapEntry``
// will always get destroyed from the free-store but not the ``MapEntry``
// container.
void _MapFreeEntryDeep(MapEntry* const mapentry);

// Returns a ``Map`` instance with the built-in support for hash generation and
// key comparison regardless of their data types.
Map MapAllocStrAsKey();

// Returns a ``Map`` instance with the number of given ``entrieslen`` entries
// and with the built-in support for hash generation and key comparison
// regardless of their data types.
Map MapAllocNStrAsKey(const size_t entrieslen);

// Creates a hash from a ``key`` of ``string`` data type.
//
// This functionality allow us to place a ``key`` inside our ``Map`` provided
// that the given ``key`` is a ``string`` data type.  We read ``keylen`` bytes
// from the given ``key`` and accumulate a ``hash`` value.
hash_t Hash(const void* key);

// Compares the eqaulity of two ``keys`` of ``string`` data type.
//
// We compare ``key1`` with ``key2`` to create a result.  Key should be of
// ``string`` data type and must have a ``NULL`` terminator character.
bool_t KeyCmp(const void* key1, const void* key2);

#ifdef __cplusplus
}
#endif

#include "data/map/iterators.h"
#include "data/map/ops.h"

#endif  // CJSON_INCLUDE_DATA_MAP_MAP_H_
