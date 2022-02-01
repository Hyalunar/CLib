#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "../siphash/siphash.c"

int siphashtest()
{
	bit128_t hashkey;
	hashkey.first  = 0x0706050403020100;
	hashkey.second = 0x0f0e0d0c0b0a0908;
	char message[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e};
	unsigned int length = 15;
	//uint64_t words[] = {0, 0, 0, 0};
	
	//siphash_initialize(&hashkey, words);
	//assert(words[0] == 0x7469686173716475);
	//assert(words[1] == 0x6b617f6d656e6665);
	//assert(words[2] == 0x6b7f62616d677361);
	//assert(words[3] == 0x7b6b696e727e6c7b);
	//words[3] ^= 0x0706050403020100;
	//assert(words[3] == 0x7c6d6c6a717c6d7b);
	//DOUBLE_ROUND(words[0], words[1], words[2], words[3]);
	//siphash_round(words);
	//siphash_round(words);
	//assert(words[0] == 0x4d07749cdd0858e0);
	//assert(words[1] == 0x0d52f6f62a4f59a4);
	//assert(words[2] == 0x634cb3577b01fd3d);
	//assert(words[3] == 0xa5224d6f55c7d9c8);

	uint64_t value = siphash(2, 4, message, length, &hashkey);
	assert(value == 0xa129ca6149be45e5);
}
