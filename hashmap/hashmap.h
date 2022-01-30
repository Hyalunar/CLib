/*
 * HashMap.h
 *
 *  Created on: Jan 23, 2022
 *      Author: Sleppo04
*/
#include "../utils/types.h"
#include <stdbool.h>

#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct HashMapEntry {
	unsigned char* key;
	int keyLength;
	void* value;
	struct HashMapEntry* next;
} hashmapentry_t;

typedef struct HashMap
{
	hashmapentry_t* buckets;
	bit128_t hashKey;
	int capacity;
	int size;
} hashmap_t;

/* HashMap MAIN Functions */
hashmap_t* HashMapNew();
hashmap_t* HashMapNewCapacity(int capacity);
int HashMapPut(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength, void* value);
int HashMapHasKey(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength);
int HashMapGetUsedBuckets(hashmap_t* hashMap);
void* HashMapGet(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength);
int HashMapSet(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength, void* value);
int HashMapSet_(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength, void* value, bool freeValue);
int HashMapRemove(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength);
int HashMapRemove_(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength, bool freeValue);
void HashMapFree(hashmap_t* hashMap);
// ===UTILS====


#endif // HASHMAP_H
