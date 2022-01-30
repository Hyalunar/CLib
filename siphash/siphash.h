#include <stdint.h>
#include "../utils/types.h"

#ifndef SIPHASH_H
#define SIPHASH_H

uint64_t siphash(int c, int d, char* key, unsigned int keyLength, bit128_t* hashkey);

#endif /*SIPHASH_H*/
