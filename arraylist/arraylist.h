/*
 * ArrayList.h
 *
 *  Created on: Jan 19, 2022
 *      Author: luca
*/
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#ifndef SRC_ARRAYLIST_ARRAYLIST_H_
#define SRC_ARRAYLIST_ARRAYLIST_H_

/* How to Use the ArrayList:
 * ArrayList_new creates a new ArrayList struct with initialized values.
 * ArrayList_new_capacity creates a new ArrayList struct with a set capacity of the array.
 * ArrayList_change_capacity tries to adjust the capacity to a given new_capacity:
 *  - returns true on success, false if failed.
 * ArrayList_append appends val to the end of the array, if needed, capacity will be increased.
 * ArrayList_remove_nofree removes the element at index, but does not free it.
 * ArrayList_remove removes the element at index and does free it.
 * ArrayList_remove_ removes the element at index but frees it only if free_element.
 * ArrayList_free_without_elements frees the struct and the array, but not the contents of the array.
 * ArrayList_free frees the struct, the array and the contents of the array.
 * ArrayList_insert inserts val at index. All Elements from index to size will be moved to the right
 *  - if needed, a resize will take place -> returns false on fail
 * ArrayList_extend_with_list will take another ArrayList and add all the values of src_list into dest_list.
 *  - if needed, a resize will take place -> returns false on fail
 * ArrayList_get returns the element at index if within size, raises SIGSEGV otherwise.
 * ArrayList_pop does ArrayList_get(index) and ArrayList_remove(index) in that order.
 */

typedef struct ArrayList
{
	void** array;
	int size;
	int capacity;
} arraylist_t;

arraylist_t* ArrayList_new_capacity(int initialcapacity);
arraylist_t* ArrayList_new();
bool ArrayList_change_capacity(arraylist_t* list, int new_capacity);
bool ArrayList_append(arraylist_t* list, void* val);
bool ArrayList_remove_nofree(arraylist_t* list, int index);
bool ArrayList_remove(arraylist_t*, int index);
bool ArrayList_remove_(arraylist_t* list, int index, bool free_element);
bool ArrayList_free_without_element(arraylist_t* list);
bool ArrayList_free(arraylist_t* list);
bool ArrayList_insert(arraylist_t* list, void* val, int index);
bool ArrayList_extend_with_list(arraylist_t* dest_list, arraylist_t* source_list);
void* ArrayList_get(arraylist_t* list, int index);
void* ArrayList_pop(arraylist_t* list, int index);

#define DECLARE_ARRAYLIST(type, typename) \
	typedef struct { \
	type* array; \
	int size; \
	int capacity; \
	} typename; \
	\
	typename* typename##_new_capacity(int initialcapacity) \
	{ \
		typename* new_list = malloc(sizeof(typename)); \
		new_list->capacity = initialcapacity; \
		new_list->size = 0; \
		new_list->array = malloc(sizeof(type) * new_list->capacity); \
		return new_list; \
	} \
	\
	typename* typename##_new() \
	{ \
		return typename##_new_capacity(10); \
	} \
	\
	bool typename##_change_capacity(typename* list, int new_capacity) \
	{ \
		type* new_array = realloc(list->array, sizeof(type) * new_capacity); \
		\
		if (!new_array) \
		{ \
			/* Null pointer returned? */ \
			return false; \
		} else \
		{ \
			/* Functional pointer returned? Replace Old pointer with new one. */ \
			list->array = new_array; \
			list->capacity = new_capacity; \
			return true; \
		} \
	} \
	\
	bool typename##_append(typename* list, type val) \
	{ \
		/* Returns NULL if the pointer is NULL.
		 * Returns true if the append succeeded.
		 * Returns false if a needed capacity increase failed.
		 * Time Complexity of this operation is:
		 *  -  O(1) if no increase of capacity is needed.
		 *  -  O(n) if the capacity needed to be increased.
		 */ \
		if (!list) \
		{ \
			return NULL; \
		} \
		if(list->size + 1 <= list->capacity) \
		{ \
			list->array[list->size] = val; \
			list->size += 1; \
			return true; \
			\
		} else \
		{ \
			int new_capacity = list->capacity * 2; \
			if(typename##_change_capacity(list, new_capacity)) \
			{ \
				list->capacity = new_capacity; \
				list->array[list->size] = val; \
				list->size += 1; \
				return true; \
			} else \
			{ \
				/* Capacity increase failed */ \
				return false; \
			} \
		} \
	} \
	\
	bool typename##_remove_(typename* list, int index, bool free_element) \
	{ \
		/* Returns NULL if the pointer is NULL.
		 *  Returns true if the remove succeeded.
		 *  Returns false if the index is out of range.
		 *  Time Complexity is O(n), where n is the size of the list.
		 */ \
		if(!list) \
		{ \
			return NULL; \
		} \
		\
		if (index >= list->size) \
		{ \
			/* Out of index */ \
			return false; \
		} \
		\
		if(free_element) \
		{ \
			free(list->array[index]); \
			list->array[index] = NULL; \
		} \
		for (int i = index; i < list->size - 1; i++) \
		{ \
			list->array[i] = list->array[i + 1]; \
		} \
		list->size -= 1; \
		if (list->size < list->capacity / 2) \
		{ \
			int new_capacity = list->capacity / 2 * 1.25; \
			typename##_change_capacity(list, new_capacity); \
		} \
		return true; \
	} \
	bool typename##_remove_nofree(typename* list, int index) \
	{ \
		return typename##_remove_(list, index, false); \
	} \
	\
	bool typename##_remove(typename* list, int index) \
	{ \
		return typename##_remove_(list, index, true); \
	} \
	\
	\
	bool typename##_free_without_elements(typename* list) \
	{ \
		if (!list) \
		{ \
			return NULL;\
		} \
		\
		free(list->array); \
		free(list); \
		list = NULL; \
		return true; \
	} \
	\
	bool typename##_free(typename* list) \
	{ \
		if (!list) \
		{ \
			return NULL;\
		} \
		for (int index = 0; index < list->size; index++) \
		{ \
			free(list->array[index]); \
		} \
		free(list->array); \
		free(list); \
		list = NULL; \
		return true; \
	} \
	\
	bool typename##_insert(typename* list, type val, int index) \
	{ \
		/* Returns NULL if the pointer is NULL
		 * Returns true if the insert succeeded
		 * Returns false if the index was out of range or the insert failed
		 * (due to a failed capacity increase)
		 * Time Complexity of this operation is O(n)
		 */ \
		if (!list) \
		{ \
			return NULL; \
		} \
		\
		if (index < 0 || index >= list->size) \
		{ \
			return false;\
		} \
		\
		if (list->size + 1 > list->capacity) \
		{ \
			if (typename##_change_capacity(list, list->size * 2)) \
			{ \
				/* Increase failed */ \
				return false; \
			} \
		} \
		\
		for (int move_index = list->size; move_index > index; move_index--) \
		{ \
			list->array[move_index] = list->array[move_index - 1]; \
		} \
		list->array[index] = val; \
		return true; \
	} \
	\
	bool typename##_extend_with_list(typename* dest_list, typename* source_list) \
	{ \
		/* Returns NULL if one of the pointers is NULL.
		 * Returns true if the extend succeeded.
		 * Returns false if the extend failed
		 * (due to a failed capacity increase)
		 * Time complexity of this operation is:
		 * -  O(n + m) if a capacity increase is needed, where n is the size of dest_list and m is the size of source_list
		 * -  O(n) if no capacity increase is needed, where n is the size of source_list.
		 */ \
		if (!dest_list || !source_list)\
		{ \
			return NULL;\
		} \
		if (dest_list->size + source_list->size > dest_list->capacity) \
		{ \
			int new_capacity = dest_list->size + source_list->size + 10; \
			if (!(typename##_change_capacity(dest_list, new_capacity))) \
			{ \
				return false;\
			} \
		} \
		for (int index = 0; index < source_list->size; index++) \
		{ \
			typename##_append(dest_list, source_list->array[index]); \
		} \
		return true; \
	} \
	\
	type typename##_get(typename* list, int index) \
	{ \
		/* Returns NULL if the pointer is NULL
		 * Returns list->array[index] if index is valid, otherwise NULL
		 */ \
		if(!list) return NULL; \
		if(index >= 0 && index < list->size) \
		{ \
			return list->array[index]; \
		} else \
		{ \
			raise(SIGSEGV); \
		} \
	} \
	\
	type typename##_pop(typename* list, int index) \
	{ \
		if(!list) return NULL; \
		if(index >= 0 && index < list->size) \
		{ \
			type val = typename##_get(list, index); \
			typename##_remove(list, index); \
			return val; \
		} else { \
			raise(SIGSEGV); \
		} \
	}

#endif /* SRC_ARRAYLIST_ARRAYLIST_H_ */
