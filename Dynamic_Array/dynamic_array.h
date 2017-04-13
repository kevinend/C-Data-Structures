#ifndef DYNAMIC_ARRAY_H_
#define DYNAMIC_ARRAY_H_

#include <stdbool.h>

/**** Library Usage Notes ****/
/*
 *  If a function returns "ownership" of some data that means that the data is allocated on the heap and *you* are responsible for freeing it.
 *  The user must define a free function if you array_add() elements which are pointers to owned data
 *  The prototype for the freeing function is:
 *      void(*freefn)(void *) -- note that if the dynamic array's elements are pointers to heap allocated data, the void * returned by the 
 *							  -- functions darray_remove_last() and darray_unordered_remove() can be used directly in the `freefn` however
 *								 they will need to be casted to the double pointer (your_type **) within the definition of your freeing function
 *								 to properly release the memory.
 *  For non-owning types (such as a dynamic_array of ints, floats, etc), anywhere there exists a parameter for a freeing function, you can just
 *    pass in NULL and it should operate appropriately.
 *
 *	darray_remove_last()     -- user takes ownership of the data if it lives on the heap
 *  darray_unoreded_remove() -- user takes ownership of the data if it lives on the heap 
 *  darray_shrink_to_fit()   -- the `capacity` exceeds the `count` in two ways, either allocated extra space (waste, no free necessary) or you used 
 *						        darray_remove_last() or darray_unordered_remove(). In either case you as the user are responsible to free that data 
 *                              therefore no freeing function is associated with this function
 *  darray_shrink_to_size()  -- accepts a freeing function because a new size that is smaller than the `count` would leave owning pointers on the 
 *							    dynamic array that would become inaccessible and thus leak memory
 *  darray_clear()           -- takes a freeing function since clearing the buffer essentially just resets the `count` to zero allowing you to overwrite
 *								previous elements. Although efficient, if you don't free the previous elements you will leak memory
 *
 */

 /* TODO: fix shrink_to_size so that the next array_push doesn't lead to another immediate reallocation */

typedef struct {
	void *buffer;
	size_t count;
	size_t capacity;
	size_t element_size;
} Dynamic_Array; 

Dynamic_Array darray_init( size_t );

Dynamic_Array darray_init_with_capacity( size_t, size_t );

void darray_add( void *, Dynamic_Array * );

void * darray_remove_last( Dynamic_Array * );

bool darray_is_empty( Dynamic_Array * );

void * darray_at_index( size_t, Dynamic_Array * );

void * darray_unordered_remove( size_t, Dynamic_Array * );

void darray_shrink_to_fit( Dynamic_Array * );

void darray_shrink_to_size( size_t, Dynamic_Array *, void(*)(void *) );

void darray_clear( Dynamic_Array *, void(*)(void *) );

void darray_free_all_elements( size_t, Dynamic_Array *, void(*)(void *) ) ;

void darray_free( Dynamic_Array * ); 

#endif
