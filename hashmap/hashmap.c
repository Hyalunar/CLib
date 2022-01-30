#include <string.h>
#include "hashmap.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include "../siphash/siphash.h"
#include <stdbool.h>

bit128_t _generate_bit128Key()
{
	bit128_t bitKey;
	unsigned char* bitKeyLocation = (unsigned char*) &bitKey;
	int byte4Rand;
	srand(time(0));
	for (int i = 0; i < (16 / sizeof(int)); i++) {
		byte4Rand = rand();
		memcpy(bitKeyLocation + i * sizeof(int), &byte4Rand, sizeof(int));
	}
	return bitKey;
}

hashmapentry_t* HashMapEntryNew(unsigned char* key, unsigned int keyLength, void* value)
{
	hashmapentry_t* entry = malloc(sizeof(hashmapentry_t));
	entry->key = malloc(sizeof(unsigned char) * keyLength);
	memcpy(entry->key, key, sizeof(unsigned char) * keyLength);
	entry->keyLength = keyLength;
	entry->value = value;
	entry->next = NULL;
	return entry;
}

hashmap_t* HashMapNewCapacity(int capacity)
{
	hashmap_t* new_map 	= malloc(sizeof(hashmap_t));
	new_map->buckets 	= calloc(capacity, sizeof(hashmapentry_t));
	new_map->capacity 	= capacity;
	new_map->hashKey 	= _generate_bit128Key();
	new_map->size 		= 0;
	return new_map;
}

hashmap_t* HashMapNew()
{
	return HashMapNewCapacity(10);
}

uint64_t getHash(unsigned char* key, unsigned int keyLength, bit128_t hashKey)
{
	int cRounds = 2;
	int dRounds = 4;
	return siphash(cRounds, dRounds, (char*) key, keyLength, &hashKey);
}

hashmapentry_t* HashMap_getBucketNode(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength)
{
	uint64_t hash = getHash(key, keyLength, hashMap->hashKey);
	int valuePos = hash % hashMap->capacity;
	hashmapentry_t* node = hashMap->buckets + valuePos;
	return node;
}

int keyEquality(unsigned char* key1, unsigned int key1Length, unsigned char* key2, unsigned int key2Length)
{
	/* Returns 1 if keys are equal, otherwise 0 */
	if(key1Length != key2Length) return 0;
	for (int i = 0; i < key1Length; i++)
	{
		if(key1[i] != key2[i]) return 0;
	}
	return 1;
}

int HashMapPut(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength, void* value)
{
	/* Returns -1 if invalid argument was passed.
	 * Returns 0 if key is already used
	 * Returns 1 on success
	 * Returns -2 if something very weird happened
	 */
	if(!hashMap || !key || !keyLength) return -1;
	hashmapentry_t* node = HashMap_getBucketNode(hashMap, key, keyLength);
	if (node->key) {
		//Already some value in place
		while(node->next) {
			//Check if key is already used
			if(keyEquality(node->key, node->keyLength, key, keyLength)) return 0;
			node = node->next;
		}
		//node is now the last node in the linkedList-Bucket
		node->next = HashMapEntryNew(key, keyLength, value);
		return 1;
	} else {
		//There is no node in this bucket
		hashmapentry_t* new_node = HashMapEntryNew(key, keyLength, value);
		*node = *new_node;
		free(new_node);
		return 1;
	}
	return -2;
}

int HashMapHasKey(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength)
{
	if(!hashMap || !key || !keyLength) return -1;
	hashmapentry_t node = *(HashMap_getBucketNode(hashMap, key, keyLength));
	while (node.key) {
		if(keyEquality(key, keyLength, node.key, node.keyLength)) return 1;
		if(!node.next) return 0;
		node = *(node.next);
	}
	return 0;
}

void* HashMapGet(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength)
{
	/* Returns the corresponding value on success, otherwise NULL */
	if(!hashMap || !key || !keyLength) return NULL;
	hashmapentry_t node = *(HashMap_getBucketNode(hashMap, key, keyLength));
	while(node.key) {
		if(keyEquality(key, keyLength, node.key, node.keyLength)) return node.value;
		if(!node.next) return NULL;
		node = *(node.next);
	}
	return NULL;
}

int HashMapRemove_(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength, bool freeValue)
{
	/* Returns -1 if one if the parameters is invalid
	 * Returns 1 on success
	 * Returns 0 if the key is not in this hashmap.
	 */
	hashmapentry_t* previousNode = NULL;
	if(!hashMap || !key || !keyLength) return -1;
	hashmapentry_t* node = HashMap_getBucketNode(hashMap, key, keyLength);
	while (! keyEquality(key, keyLength, node->key, node->keyLength)) {
		//Searching for the correct node
		if(! node->next) return 0;
		previousNode = node;
		node = node->next;
	}
	if(node->next) {
		//Node has a follower node
		free(node->key);
		node->key = NULL;
		if (freeValue) {
			free(node->value);
			node->value = NULL;
		}
		if (previousNode) {
			previousNode->next = node->next;
			free(node);
		}
		return 1;
	} else {
		/* Node hasn't got a follower node. */
		free(node->key);
		if (freeValue) {
			free(node->value);
			node->value = NULL;
		}
		node->key = NULL;
		if (previousNode) {
			free(node);
			previousNode->next = NULL;
		}
		return 1;
	}
	return 0;
}

int HashMapRemove(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength) {
	return HashMapRemove_(hashMap, key, keyLength, true);
}

int HashMapSet_(hashmap_t* hashMap, unsigned char* key, unsigned int keyLength, void* value, bool freeValue)
{
	/* Returns 0 if the key is not in this hash-map
	 * Returns -1 if one of the inputs is invalid
	 * Returns 1 on success
	 */
	if(!hashMap || !key || !keyLength) return -1;
	hashmapentry_t* node = HashMap_getBucketNode(hashMap, key, keyLength);
	while(node->key) {
		if(keyEquality(key, keyLength, node->key, node->keyLength)) {
			if (free) free(node->value);
			node->value = value;
			return 1;
		}
	}
	return 0;
}

int HashMapSet (hashmap_t* hashMap, unsigned char* key, unsigned int keyLength, void* value)
{
	return HashMapSet_(hashMap, key, keyLength, value, true);
}

int HashMapGetUsedBuckets(hashmap_t* hashMap)
{
	int used = 0;
	for (int i = 0; i < hashMap->capacity; i++) {
		used += (hashMap->buckets[i].key != NULL);
	}
	return used;
}

void HashMapFree_(hashmap_t* hashMap, bool freeValues)
{
	hashmapentry_t* node;
	hashmapentry_t* nextNode;
	hashmapentry_t* next2Node;
	for (int bucket_idx = 0; bucket_idx < hashMap->capacity; bucket_idx++) {
		node = hashMap->buckets + bucket_idx;
		if(freeValues && node->key) free(node->value);
		free(node->key);
		if(node->next) {
			nextNode = node->next;
			while(nextNode->next) {
				next2Node = nextNode->next;
				if(freeValues) free(nextNode->value);
				free(nextNode->key);
				free(nextNode);
				nextNode = next2Node;
			}
			if(freeValues) free(nextNode->value);
			free(nextNode->key);
			free(nextNode);
		}
	}
	free(hashMap->buckets);
	free(hashMap);
	return;
}

void HashMapFree(hashmap_t* hashMap)
{
	HashMapFree_(hashMap, true);
}

int HashMapItemCount(hashmap_t* hashMap)
{
	int count = 0;
	for (int bucket_idx = 0; bucket_idx < hashMap->capacity; bucket_idx++) {
		hashmapentry_t* node = hashMap->buckets + bucket_idx;
		while (node->key) {
			count++;
			if (node->next) {
				node = node->next;
			} else {
				break;
			}
		}
	}
	return count;
}


