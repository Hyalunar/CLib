#include "../hashmap/hashmap.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

int main()
{
	hashmap_t* map = HashMapNewCapacity(65000);

	unsigned char* key2 = calloc(2, sizeof(char));
	for (key2[0] = 0; key2[0] != 254; (key2[0])++) {
		for(key2[1] = 0; key2[1] != 254; (key2[1])++) {
			int* val = malloc(sizeof(int));
			*val = (key2[0] * key2[1]);
			HashMapPut(map, key2, (unsigned int) 2, val);
			assert(HashMapHasKey(map, key2, 2));
		}
	}

	for (key2[0] = 0; key2[0] != 254; (key2[0])++) {
		for(key2[1] = 0; key2[1] != 254; (key2[1])++) {
			assert(HashMapHasKey(map, key2, 2));
			assert(*((int*) HashMapGet(map, key2, 2)) == key2[0] * key2[1]);
		}
	}

	assert(HashMapItemCount(map) == 64516);
	key2[0] = 'a';
	key2[1] = 'b';
	HashMapRemove(map, key2, 2);
	assert(HashMapItemCount(map) == 64515);

	unsigned char* long_key = malloc(sizeof(char) * 100000);
	memset(long_key, 0xDEADBEEF, 100000);
	assert(! HashMapHasKey(map, long_key, 100000));
	free(long_key);


	HashMapResize(&map, 100000);
	HashMapFree(map);
	free(key2);
}
