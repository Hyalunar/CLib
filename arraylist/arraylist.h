#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <stdint.h>

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
	uint32_t size;
	int capacity;
} arraylist_t;

arraylist_t* ArrayList_new_capacity(int initialcapacity);
arraylist_t* ArrayList_new();
bool ArrayList_change_capacity(arraylist_t* list, int new_capacity);
bool ArrayList_append(arraylist_t* list, void* val);
bool ArrayList_remove_nofree(arraylist_t* list, int index);
bool ArrayList_remove(arraylist_t*, int index);
bool ArrayList_remove_(arraylist_t* list, int index, bool free_element);
bool ArrayList_free_without_elements(arraylist_t* list);
bool ArrayList_free(arraylist_t* list);
bool ArrayList_insert(arraylist_t* list, void* val, int index);
bool ArrayList_extend_with_list(arraylist_t* dest_list, arraylist_t* source_list);
void* ArrayList_get(arraylist_t* list, int index);
void* ArrayList_pop(arraylist_t* list, int index);

#endif /* SRC_ARRAYLIST_ARRAYLIST_H_ */
