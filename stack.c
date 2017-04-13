#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "stack.h"

Stack
stack_init( size_t element_size ) 
{
	Stack stack;
	stack.data = darray_init( element_size );
	
	return stack;
}

Stack
stack_init_with_capacity( size_t initial_capacity, size_t element_size )
{
	Stack stack;
	stack.data = darray_init_with_capacity( initial_capacity, element_size );

	return stack;
}

void
stack_push( void *element, Stack *stack ) 
{
	darray_add( element, &(stack->data) );
	return;
}

void *
stack_pop( Stack *stack ) 
{
	void *popped_element;
	popped_element = darray_remove_last( &(stack->data) );

	return popped_element;
}

bool
stack_is_empty( Stack *stack )
{
	return darray_is_empty( &(stack->data) );
}

void
stack_shrink_to_fit( Stack *stack ) 
{
	darray_shrink_to_fit( &(stack->data) );
	return;
}

void
stack_shrink_to_size( size_t new_size, Stack *stack, void(*freefn)(void *) ) 
{
	darray_shrink_to_size( new_size, &(stack->data), freefn );
	return;
}

void
stack_clear( Stack *stack, void(*freefn)(void *) )
{
	darray_clear( &(stack->data), freefn );
	return;
}

void
stack_free_all_elements( Stack *stack, void(*freefn)(void *) ) 
{
	darray_free_all_elements( 0, &(stack->data), freefn );
	return;
}

void
stack_free( Stack *stack ) 
{
	darray_free( &(stack->data) );
	return;
}

size_t
stack_count( Stack *stack )
{
	return stack->data.count;
}

size_t 
stack_capacity( Stack *stack )
{
	return stack->data.capacity;
}







