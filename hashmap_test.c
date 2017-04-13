#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "hashmap.h"

/*** User needs to define a free function and compare function -- just using strcmp for the comparison function ***/

Hashmap hashmap;

void print_count_and_capacity( void ) 
{
	printf( "hashmap count == %zd, capacity == %zd\n\n", hashmap_count( &hashmap ), hashmap_capacity( &hashmap ) );
	return;
}

char *allocate_string( char *input ) 
{
	char *new_string;
	size_t length;

	length = strlen( input );

	new_string = (char *)malloc( length + 1);
	strncpy( new_string, input, length );
	new_string[length] = '\0';

	return new_string;
}

void free_string( void *string )
{
	free( string );
	return;
}

int main( void ) {

	hashmap = hashmap_init( sizeof (char *), sizeof (int) );
	print_count_and_capacity();


	return 0;
}
