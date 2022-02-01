#include "siphash.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define ROTATE(b, n) ((b) << n | (b) >> (64 - n))

void siphash_initialize(bit128_t* key, uint64_t* words)
{
	words[0] = key->first  ^ 0x736f6d6570736575;
	words[1] = key->second ^ 0x646f72616e646f6d;
	words[2] = key->first  ^ 0x6c7967656e657261;
	words[3] = key->second ^ 0x7465646279746573;
	/* I don't like it either, just look at https://cr.yp.to/siphash/siphash-20120727.pdf */
}

void siphash_round(uint64_t* words)
{

	words[0] += words[1];				words[2] += words[3];
	words[1] = ROTATE(words[1], 13);	words[3] = ROTATE(words[3], 16);
	words[1] ^= words[0];				words[3] ^= words[2];
	words[0] = ROTATE(words[0], 32);
	words[2] += words[1];				words[0] += words[3];
	words[1] = ROTATE(words[1], 17);	words[3] = ROTATE(words[3], 21);
	words[1] ^= words[2];				words[3] ^= words[0];
	words[2] = ROTATE(words[2], 32);
}

uint64_t siphash_finalize(uint64_t* words, int d)
{
	uint64_t hashValue;
	
	words[2] ^= 0xff;
	while (d) {
		siphash_round(words);
		d--;
	}

	// Das muss so. Echt jetzt.
	hashValue = words[0] ^ words[1] ^ words[2] ^ words[3];
	return hashValue;
}

void perform_rounds(int times, uint64_t* words)
{
	for (int k = 0; k < times; k++) {
		// Perform round on this word
		siphash_round(words);
	}
}

void siphash_compress(int c, char* value, unsigned int valueLength, uint64_t* words)
{
	// c is a parameter for the amount of siphash_rounds to be done
	uint64_t* byteString  = (uint64_t*) value;
	int       wordcount   = (valueLength + 1 ) / 8;
	uint64_t  lastWord    = 0;
	uint64_t  current;
	
	for (int i = 0; i < wordcount - 1; i++) {
		// Minus one because the last word is somewhat special
		current = *byteString;
		words[3] ^= current;
		
		perform_rounds(c, words);
		
		// Append last special byte
		words[0] ^= current;
		byteString += 1; // Move pointer to next 64 bits
	}

	// FF 00 AB CF 00 00 00 (valueLength % 256)
	int remainingBytes = valueLength % 8;
	memcpy(&lastWord, byteString, remainingBytes);
	uint64_t compareValue = valueLength % 256;
	compareValue <<= 56; // WTF is this thing? Don't touch, it works!!elf1!
	lastWord |= compareValue; //(unsigned char) (valueLength % 256);
	
	words[3] ^= lastWord;
	perform_rounds(c, words);
	words[0] ^= lastWord;

}

uint64_t siphash(int c, int d, char* key, unsigned int keyLength, bit128_t* hashkey)
{
	/* C and D are parameters of siphash */
	uint64_t words[] = {0, 0, 0, 0};
	siphash_initialize(hashkey, words);
	siphash_compress(c, key, keyLength, words);
	uint64_t hashValue = siphash_finalize(words, d);
	return hashValue;
}
