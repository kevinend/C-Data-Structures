#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "dynamic_array.h"

/*** NOTE: This hashmap implmentation was just experimental  ***/
/* It does not handle duplicates entered and would have terrible performance with a large number of entries
 *
 * Also, the implementation is sloppy in C. This is a by-product of trying to be fully generic in both the keys and values
 *
 * I have an idea how to clean this up using Jon Blow's new language JAI (currently unreleased) which have better features
 * to support this kind of hashmap structure.
 */ 

typedef struct {
	Dynamic_Array keys;
	Dynamic_Array values;
} Hashmap;

Hashmap hashmap_init( size_t, size_t );

Hashmap hashmap_init_with_capacity( size_t, size_t, size_t );

void hashmap_insert( void *, void *, Hashmap * );

void hashmap_remove( void *, Hashmap *, int(*)(void *, void *) );

bool hashmap_contains_key( void *, Hashmap *, int(*)(void *, void *) );

void hashmap_clear( Hashmap *, void(*)(void *), void(*)(void *) );


void * hashmap_key_at_index( size_t, Hashmap * );

void * hashmap_value_at_index( size_t, Hashmap * );

void hashmap_free_all_keys( Hashmap *, void(*)(void *) );

void hashmap_free_all_values( Hashmap *, void(*)(void *) );

void hashmap_free( Hashmap * );

size_t hashmap_count( Hashmap * ); 

size_t hashmap_capacity( Hashmap * );

#endif



