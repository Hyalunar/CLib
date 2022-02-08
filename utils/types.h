#include <stdint.h>

#ifndef UTILS_TYPES_H
#define UTILS_TYPES_H

typedef struct Bit128 {
	uint64_t first;
	uint64_t second;
} bit128_t;

typedef struct Bytes {
	int length;
	unsigned char** bytes;
} bytes_t;

#endif
