#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "hashmap.h"

Hashmap
hashmap_init( size_t key_size,
			  size_t value_size )
{
	Hashmap hashmap;

	hashmap.keys = darray_init( key_size );
	hashmap.values = darray_init( value_size );

	return hashmap;
}

Hashmap
hashmap_init_with_capacity( size_t initial_capacity,
							size_t key_size,
							size_t value_size )
{
	Hashmap hashmap;

	hashmap.keys = darray_init_with_capacity( initial_capacity, key_size );
	hashmap.values = darray_init_with_capacity( initial_capacity, value_size );

	return hashmap;
}

void
hashmap_insert( void *key, void *value, Hashmap *hashmap, int(*cmpfn)(void *, void *) )
{
	darray_add( key, &(hashmap->keys) );
	darray_add( value, &(hashmap->values ) );

	return;
}


static void
hashmap_swap_both_keys_and_values( size_t removal_index, size_t second_index, Hashmap *hashmap )
{
	darray_unordered_remove( removal_index, &(hashmap->keys) );
   	darray_unordered_remove( removal_index, &(hashmap->values) ); 
  
  	return;
}

void 
hashmap_remove( void *key, Hashmap *hashmap, int(*cmpfn)(void *, void *) )
{
	void *hashmap_entry;
	size_t i = 0;
	while ( i < hashmap->keys.count ) {
		hashmap_entry = hashmap_key_at_index( i, hashmap );

		if ( cmpfn( key, hashmap_entry ) == 0 ) {
			hashmap_swap_both_keys_and_values( i, hashmap->keys.count - 1, hashmap );
			hashmap->keys.count--;
			hashmap->values.count--;
			break;
		}
	}

	return;
}
         
bool
hashmap_contains_key( void *key, Hashmap *hashmap, int(*cmpfn)(void *, void*) )
{
	bool found;
	void *hashmap_entry;
	
	found = false;
	for (size_t i = 0; i < hashmap->keys.count; ++i ) {
		hashmap_entry = hashmap_key_at_index( i, hashmap );
		if ( cmpfn( key, hashmap_entry ) == 0 ) {
			found = true;
			break;
		}
	}

	return found;
}

void
hashmap_clear( Hashmap *hashmap, void(keys_freefn)(void *), void(values_freefn)(void *) )
{
	darray_clear( &(hashmap->keys), keys_freefn );
	darray_clear( &(hashmap->values), values_freefn );
	
	return;
}

void *
hashmap_key_at_index( size_t index, Hashmap *hashmap )
{
	return darray_at_index( index , &(hashmap->keys) );
}

void *
hashmap_value_at_index( size_t index, Hashmap *hashmap )
{
	return darray_at_index( index, &(hashmap->values) );
}

void
hashmap_free_all_keys( Hashmap *hashmap, void(*freefn)(void *) )
{
	darray_free_all_elements( 0, &(hashmap->keys), freefn );
	return;
}

void
hashmap_free_all_values( Hashmap *hashmap, void(*freefn)(void *) )
{
	darray_free_all_elements( 0, &(hashmap->values), freefn );
	return;
}

void
hashmap_free( Hashmap *hashmap )
{
	darray_free( &(hashmap->keys) );
	darray_free( &(hashmap->values) );

	return;
}

size_t 
hashmap_count( Hashmap *hashmap ) 
{
	return hashmap->keys.count;
}

size_t
hashmap_capacity( Hashmap *hashmap )
{
	return hashmap->keys.capacity;
}



