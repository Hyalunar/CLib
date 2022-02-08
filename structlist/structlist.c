#include "structlist.h"

structlist_t* structListNew(uint32_t structSize)
{
	return structListNewCapacity(10);
}

structlist_t* structListNewCapacity(uint32_t capacity, uint32_t structSize)
{
	structlist_t* list = malloc(sizeof(structlist_t));
	if (!list) return NULL;
	list->size = 0;
	list->capacity = 0;
	list->structSize = structSize;
	list->array = NULL;
	list->array = malloc(structSize * capacity);
	if (!list->array) {
		free(list);
		return NULL;
	}
	return list;
}
