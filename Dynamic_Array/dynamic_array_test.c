#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "dynamic_array.h"

struct Test {
	int field1;
	int field2;
	int field3;
};

Dynamic_Array darray1;
Dynamic_Array darray2;

void print_count_and_capacity( void ) 
{
	printf( "darray1 count == %zd, capacity == %zd\n", darray1.count, darray1.capacity );
	printf( "darray2 count == %zd, capacity == %zd\n\n", darray2.count, darray2.capacity );

	return;
}

void print_elements_using_at_index( void )
{
	printf( "Elements using at_index: \n" );
	puts( "darray1: " );
	int *values1;
	for ( int i = 0; i < darray1.count; ++i ) {
		values1 = (int *)darray_at_index( i, &darray1 );
		printf( "%d ", *values1 );
	}
	printf( "\n\n" );

	puts( "darray2: " );
	struct Test **values2;
	for ( int i = 0; i < darray2.count; ++i ) {
		values2 = (struct Test **)darray_at_index( i, &darray2 );
		printf( "Field1 == %d, Field2 == %d, Field3 == %d\n", (*values2)->field1, (*values2)->field2, (*values2)->field3 );
	}
	putchar( '\n' );

	return;
}

void print_elements_using_remove( void )
{
	printf( "Elements using remove: \n" );
	puts( "darray1: " );
	int *values1;
	while ( !darray_is_empty( &darray1 ) ) {
		values1 = (int *)darray_remove_last( &darray1 );
		printf( "%d ", *values1 );
	}
	printf( "\n\n" );

	puts( "darray2: " );
	struct Test **values2;
	while ( !darray_is_empty( &darray2 ) ) {
		values2 = (struct Test **)darray_remove_last( &darray2 );	
		printf( "Field1 == %d, Field2 == %d, Field3 == %d\n", (*values2)->field1, (*values2)->field2, (*values2)->field3 );
		free( *values2 );
	}
	putchar( '\n' );

	return;
}

void free_test_struct( void *element ) 
{
	struct Test **value = (struct Test **)element;
	free( *value );
	return;
}

/* THIS SHOULD NOT LEAK!!! */
int main( void )
{
	darray1 = darray_init( sizeof (int) );
	darray2 = darray_init_with_capacity( 10, sizeof (int *) );

	// i ignore malloc failures in the following tests!!!
	struct Test *test1 = (struct Test *)malloc( 1 * sizeof (struct Test) );
	struct Test *test2 = (struct Test *)malloc( 1 * sizeof (struct Test) );
	struct Test *test3 = (struct Test *)malloc( 1 * sizeof (struct Test) );

	test1->field1 = 10;
	test1->field2 = 11;
	test1->field3 = 12;

	test2->field1 = 20;
	test2->field2 = 21;
	test2->field3 = 22;

	test3->field1 = 30;
	test3->field2 = 31;
	test3->field3 = 32;

	printf( "(1)Baseline count and capacity\n" );
	print_count_and_capacity();

	// Add simple numbers to a dynamic array
	for ( int i = 0; i < 10; ++i ) {
		darray_add( &i, &darray1 );
	}

	// Add data located on the heap 
	darray_add( &test1, &darray2 );
	darray_add( &test2, &darray2 );
	darray_add( &test3, &darray2 );

	// Testing out of bounds indexing for at_index
	printf( "(2)Testing out of bounds errors for darray_at_index()\n" );
	darray_at_index( 100, &darray1 );
	darray_at_index( -10, &darray1 );
	putchar( '\n' );
	
	print_elements_using_at_index();
	print_count_and_capacity();
	print_elements_using_remove();
	print_count_and_capacity();


	// Add a fewer than darray1 capacity numbers back -- capacity is 16
	for ( int i = 0; i < 5; ++i ) {
		darray_add( &i, &darray1 );
	}

	// Testing shrink functions the following two calls, too big and too small should fail, darray should be untouched
	printf( "(3)Failed shrink to size...test1 \n" );
	darray_shrink_to_size( 50, &darray1, NULL );     
	print_count_and_capacity(); 

	printf( "(4)Failed shrink to size...test2 \n" );
	darray_shrink_to_size( -4, &darray1, NULL );
	print_count_and_capacity();

	/**** SHHIIIIIIT -- may leak with heap allocations -- have them accept function pointers to freeing functions ****/
	// Shrink to size -- capacity of 5 and count of 8
	printf( "(5)Successful shrink to size \n" );
	darray_shrink_to_size( 8, &darray1, NULL );
	print_count_and_capacity();

	// Shrink to fit -- capacity of 5 and count of 5
	printf( "(6)Successful shrink to fit \n" );
	darray_shrink_to_fit( &darray1 );
	print_count_and_capacity();

	// Need to test shrink_to_size and shrink_to_fit to make sure they don't leak memory
	struct Test *leaky;
	for ( int i = 0; i < 20; ++i ) {
		leaky = (struct Test *)malloc( 1 * sizeof *leaky );
		leaky->field1 = i;
		leaky->field2 = i + 1;
		leaky->field3 = i + 2;

		darray_add( &leaky, &darray2 );
	}

	// Testing shrink functions for leaks
	printf( "(7)Testing shrink_to_size for leaks: \n" );
	darray_shrink_to_size( 10, &darray2, free_test_struct );
	print_count_and_capacity();

	for (int i = 0; i < 5; ++i ) {
		struct Test **removed_element;
		removed_element = darray_unordered_remove( i, &darray2 );
		free_test_struct( removed_element );
	}

	printf( "(8)Testing shrink_to_fit for leaks: \n" );
	darray_shrink_to_fit( &darray2 );
	print_count_and_capacity();

	printf( "(9)Testing unorderd_remove: \n" );
	struct Test **removed_element;
	removed_element = darray_unordered_remove( 2, &darray2 );
	free_test_struct( removed_element );
	print_count_and_capacity();

	// Using clear and the freeing functions
	printf( "(10)Testing clear and the freeing functions: \n" );
	darray_clear( &darray1, NULL );
	darray_free( &darray1 );

	darray_free_all_elements( 0, &darray2, free_test_struct );
	darray_free( &darray2 );
	
	return 0;
}
