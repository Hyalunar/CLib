/*
 * ArrayList.c
 *
 *  Created on: Jan 19, 2022
 *      Author: luca
*/

#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
//TODO Change implementation to remove Signal
#include "arraylist.h"


arraylist_t* ArrayList_new_capacity(int initialcapacity)
{
	arraylist_t* new_list = malloc(sizeof(arraylist_t));
	new_list->capacity = initialcapacity;
	new_list->size = 0;
	new_list->array = malloc(sizeof(void*) * new_list->capacity);
	return new_list;
}

arraylist_t* ArrayList_new()
{
	return ArrayList_new_capacity(10);
}

bool ArrayList_change_capacity(arraylist_t* list, int new_capacity)
{
	void** new_array = realloc(list->array, sizeof(void*) * new_capacity);

	if (!new_array)
	{
		/* Null pointer returned? */
		return false;
	} else
	{
		/* Functional pointer returned? Replace Old pointer with new one. */
		list->array = new_array;
		list->capacity = new_capacity;
		return true;
	}
}

bool ArrayList_append(arraylist_t* list, void* val)
{
	/* Returns NULL if the pointer is NULL.
	 * Returns true if the append succeeded.
	 * Returns false if a needed capacity increase failed.
	 * Time Complexity of this operation is:
	 *  -  O(1) if no increase of capacity is needed.
	 *  -  O(n) if the capacity needed to be increased.
	 */
	if (!list)
	{
		return NULL;
	}
	if(list->size + 1 <= list->capacity)
	{
		list->array[list->size] = val;
		list->size += 1;
		return true;

	} else
	{
		int new_capacity = list->capacity * 2;
		if(ArrayList_change_capacity(list, new_capacity))
		{
			list->capacity = new_capacity;
			list->array[list->size] = val;
			list->size += 1;
			return true;
		} else
		{
			/* Capacity increase failed */
			return false;
		}
	}
}

bool ArrayList_remove_(arraylist_t* list, int index, bool free_element)
{
	/* Returns NULL if the pointer is NULL.
	 *  Returns true if the remove succeeded.
	 *  Returns false if the index is out of range.
	 *  Time Complexity is O(n), where n is the size of the list.
	 */
	if(!list)
	{
		return NULL;
	}

	if (index >= list->size)
	{
		/* Out of index */
		return false;
	}

	if(free_element)
	{
		free(list->array[index]);
		list->array[index] = NULL;
	}
	for (int i = index; i < list->size - 1; i++)
	{
		list->array[i] = list->array[i + 1];
	}
	list->size -= 1;
	if (list->size < list->capacity / 2)
	{
		int new_capacity = list->capacity / 2 * 1.25;
		ArrayList_change_capacity(list, new_capacity);
	}
	return true;
}
bool ArrayList_remove_nofree(arraylist_t* list, int index)
{
	return ArrayList_remove_(list, index, false);
}

bool ArrayList_remove(arraylist_t* list, int index)
{
	return ArrayList_remove_(list, index, true);
}

bool ArrayList_free_without_elements(arraylist_t* list)
{
	if (!list)
	{
		return NULL;
	}

	free(list->array);
	free(list);
	list = NULL;
	return true;
}

bool ArrayList_free(arraylist_t* list)
{
	if (!list)
	{
		return NULL;
	}
	for (int index = 0; index < list->size; index++)
	{
		free(list->array[index]);
	}
	free(list->array);
	free(list);
	list = NULL;
	return true;
}

bool ArrayList_insert(arraylist_t* list, void* val, int index)
{
	/* Returns NULL if the pointer is NULL
	 * Returns true if the insert succeeded
	 * Returns false if the index was out of range or the insert failed
	 * (due to a failed capacity increase)
	 * Time Complexity of this operation is O(n)
	 */
	if (!list)
	{
		return NULL;
	}

	if (index < 0 || index >= list->size)
	{
		return false;
	}

	if (list->size + 1 > list->capacity)
	{
		if (ArrayList_change_capacity(list, list->size * 2))
		{
			/* Increase failed */
			return false;
		}
	}

	for (int move_index = list->size; move_index > index; move_index--)
	{
		list->array[move_index] = list->array[move_index - 1];
	}
	list->array[index] = val;
	list->size += 1;
	return true;
}

bool ArrayList_extend_with_list(arraylist_t* dest_list, arraylist_t* source_list)
{
	/* Returns NULL if one of the pointers is NULL.
	 * Returns true if the extend succeeded.
	 * Returns false if the extend failed
	 * (due to a failed capacity increase)
	 * Time complexity of this operation is:
	 * -  O(n + m) if a capacity increase is needed, where n is the size of dest_list and m is the size of source_list
	 * -  O(n) if no capacity increase is needed, where n is the size of source_list.
	 */
	if (!dest_list || !source_list)
	{
		return NULL;
	}
	if (dest_list->size + source_list->size > dest_list->capacity)
	{
		int new_capacity = dest_list->size + source_list->size + 10;
		if (!(ArrayList_change_capacity(dest_list, new_capacity)))
		{
			return false;
		}
	}
	for (int index = 0; index < source_list->size; index++)
	{
		ArrayList_append(dest_list, source_list->array[index]);
	}
	return true;
}

void* ArrayList_get(arraylist_t* list, int index)
{
	/* Returns NULL if the pointer is NULL
	 * Returns list->array[index] if index is valid, otherwise NULL
	 */
	if(!list) return NULL;
	if(index >= 0 && index < list->size)
	{
		return list->array[index];
	} else
	{
		raise(SIGSEGV);
	}
}

void* ArrayList_pop(arraylist_t* list, int index)
{
	if(!list) return NULL;
	if(index >= 0 && index < list->size)
	{
		void* val = ArrayList_get(list, index);
		ArrayList_remove(list, index);
		return val;
	} else {
		raise(SIGSEGV);
	}
}
