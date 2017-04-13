#ifndef STACK_H_
#define STACK_H_

#include "dynamic_array.h"

/**** Library Usage Notes ****/
/*
 *  If a function returns "ownership" of some data that means that the data is allocated on the heap and *you* are responsible for freeing it.
 *  The user must define a free function if you stack_push() elements which are pointers to owned data
 *  The prototype for the freeing function is:
 *      void(*freefn)(void *) -- note that if the stack's elements are pointers to heap allocated data, the void * returned by the 
 *							  -- function stack_pop() can be used directly in the `freefn` however they will need to be casted to the 
 *								 double pointer (your_type **) within the definition of your freeing function to properly release the memory.
 *
 *  For non-owning types (such as a stack of ints, floats, etc), anywhere there exists a parameter for a freeing function, you can just
 *    pass in NULL and it should operate appropriately.
 *
 *  stack_pop()      	   -- user takes ownership of the data if it lives on the heap. Make sure to free or make a copy of the data before the next stack_push() or memory will be leaked 
 *  stack_shrink_to_size() -- accepts a freeing function, `count` of the stack is adjusted downwards and could leak memory if we do not free elements from `new_size` up to `count`
 *  stack_clear()          -- resets `count` to zero, must free all the previous elements to effectively reuse the buffer without leaking memory
 */

typedef struct {
	Dynamic_Array data;
} Stack;

Stack stack_init( size_t );

Stack stack_init_with_capacity( size_t, size_t );

void stack_push( void *, Stack * );

void * stack_pop( Stack * ); 

bool stack_is_empty( Stack * );

void stack_shrink_to_fit( Stack * );

void stack_shrink_to_size( size_t, Stack *, void(*)(void *) );

void stack_clear( Stack *, void(*)(void *) );

void stack_free_all_elements( Stack *, void(*)(void *) );

void stack_free( Stack * );

size_t stack_count( Stack * );

size_t stack_capacity( Stack *);

#endif 





