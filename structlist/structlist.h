
#ifndef STRUCTLIST_H
#define STRUCTLIST_H

#include <stdint.h>

typedef struct StructList {
	uint32_t capacity;
	uint32_t size;
	uint32_t structSize;
	char* array;
} structlist_t;

structlist_t* structListNew();
structlist_t* structListNewCapacity(uint32_t capacity);

#endif
