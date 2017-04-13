#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "dynamic_array.h"

Dynamic_Array
darray_init( size_t element_size ) 
{
	Dynamic_Array darray;

	darray.count = 0;
	darray.capacity = 8;
	darray.element_size = element_size;

	darray.buffer = malloc( darray.capacity * darray.element_size );

	if ( !darray.buffer ) {
		fprintf( stderr, "darray_init()...unable to allocate initial data buffer\n" );
		exit( EXIT_FAILURE );
	}

	return darray;
}

Dynamic_Array
darray_init_with_capacity( size_t initial_capacity, 
					       size_t element_size )
{
	Dynamic_Array darray;
	
	darray.count = 0;
	darray.capacity = initial_capacity;
	darray.element_size = element_size;

	darray.buffer = malloc( darray.capacity * darray.element_size );

	if ( !darray.buffer ) {
		fprintf( stderr, "darray_init_with_capacity()...unable to allocate initial data buffer\n" );
		exit( EXIT_FAILURE );
	}

	return darray;
}

void
darray_add( void *element, Dynamic_Array *darray ) 
{

	if ( darray->count == darray->capacity ) {
		darray->capacity *= 2;
		darray->buffer = realloc( darray->buffer, darray->capacity * darray->element_size );
	}

	void *element_location = (uint8_t *)darray->buffer + (darray->count * darray->element_size);

	memcpy( element_location, element, darray->element_size );
	darray->count++;

}

/* Removes the last element added to the dynamic array */
void *
darray_remove_last( Dynamic_Array *darray )
{
	if ( darray_is_empty( darray ) ) {
		return NULL;
	}

	darray->count--;

	void *popped_element;
	popped_element = (uint8_t *)darray->buffer + (darray->count * darray->element_size);

	return popped_element;
}

bool
darray_is_empty( Dynamic_Array *darray )
{
	return darray->count == 0;
}

void *
darray_at_index( size_t index, Dynamic_Array *darray ) 
{
	if ( index < 0 || index >= darray->count ) {
		fprintf( stderr, "darray_at_index()...out of bounds exception\n" );
		return NULL;
	}

	uint8_t *data_buffer = (uint8_t *)darray->buffer;

	return (data_buffer + (index * darray->element_size) );
}

static void *
swap_element_with_last( size_t swap_index, Dynamic_Array *darray )
{
	void *swap_location;
	void *last_location;

	swap_location = darray_at_index( swap_index, darray );
	last_location = darray_at_index( darray->count-1, darray );

	void *temp;

	temp = malloc( darray->element_size );

	memcpy( temp, swap_location, darray->element_size );
	memcpy( swap_location, last_location, darray->element_size );
	memcpy( last_location, temp, darray->element_size );

	free( temp );

	return last_location;
}

/* 
 * NOTE: darray_unordered_remove returns a pointer to the element being removed. If the element 
 *       is heap allocated you must capture the return value in the caller's scope and free it manually 
 *		 to avoid a leak. If the location held a copy type (such as an int) (such as an int) then 
 *       you can just ignore the return value.
 *
 *		 Also, `removal_index` bounds are handled by the call to at_index.
 */

void *
darray_unordered_remove( size_t removal_index, Dynamic_Array *darray )
{
	void *removed_element;

	removed_element = swap_element_with_last( removal_index, darray );
	darray->count--;
	return removed_element;
}

/* realloc "should" not fail since decreasing the allocation block size */
void
darray_shrink_to_fit( Dynamic_Array *darray ) 
{
	if ( darray->count < darray->capacity ) {
		darray->buffer = realloc( darray->buffer, darray->count * darray->element_size );
		darray->capacity = darray->count;
	}

	return;
}

/* realloc "should" not fail since decreasing the allocation block size */
void
darray_shrink_to_size( size_t new_size, Dynamic_Array *darray, void(*freefn)(void *) )
{
	if ( new_size < 0 || new_size >= darray->capacity ) {
		return;
	}

	if ( new_size < darray->count ) {
		if ( freefn != NULL ) {	
			for ( size_t i = new_size; i < darray->count; ++i ) {
				freefn( darray_at_index( i, darray ) );
			}
		}

		darray->count = new_size;
	}


	darray->buffer = realloc( darray->buffer, new_size * darray->element_size );
	darray->capacity = new_size;
		
	return;
}

void
darray_clear( Dynamic_Array *darray, void(*freefn)(void *) )
{
	if ( freefn != NULL ) {
		darray_free_all_elements( 0, darray, freefn );
	}

	darray->count = 0;
	return;
}

void
darray_free_all_elements( size_t starting_index, Dynamic_Array *darray, void(*free_fn)(void *) ) 
{
	uint8_t *darray_elements = darray->buffer;

	for ( int i = starting_index; i < darray->count; ++i ) {
		free_fn( darray_elements + (i * darray->element_size) );
	}

	return;
}

void 
darray_free( Dynamic_Array *darray ) 
{
	free( darray->buffer );
	return;
}



