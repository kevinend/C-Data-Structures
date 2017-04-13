#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "queue.h"

Queue queue1;
Queue queue2;

struct Test {
	int field1;
	int field2;
	int field3;
};

void print_count_and_capacity( void ) 
{
	printf( "Queue1 count == %zd, capacity == %zd, head == %zd\n", queue_count( &queue1 ), queue_capacity( &queue1 ), queue1.head );
	printf( "Queue2 count == %zd, capacity == %zd, head == %zd\n\n", queue_count( &queue2 ), queue_capacity( &queue2 ), queue2.head );
	
	return;
}

// Even though these are stack (automatic) variables, the queue will hold the correct values since they are copy types
void populate_queue1( size_t n )
{
	for ( int i = 0; i < n; ++i ) {
		queue_enqueue( &i, &queue1 );
	}

	return;
}

void populate_queue2( size_t n )
{
	struct Test *leaky;
	for ( int i = 0; i < n; ++i ) {
		leaky = malloc( 1 * sizeof *leaky );
		leaky->field1 = i;
		leaky->field2 = i + 1;
		leaky->field3 = i + 2;
	
		queue_enqueue( &leaky, &queue2 );
	}

	return;
}

void free_struct_test( void *element ) 
{
	struct Test **value = (struct Test **)element;
	
	free( *value );
	return;
}

int main( void )
{
	queue1 = queue_init( sizeof (int) );
	queue2 = queue_init_with_capacity( 10, sizeof (struct Test *) );

	print_count_and_capacity();

	printf( "(1)Testing queue_enqueue: \n" );
	populate_queue1( 5 );
	populate_queue2( 15 );
	print_count_and_capacity();

	printf( "(2)Testing queue_dequeue: \n" );
	int *values1;
	for ( int i = 0; i < 5; ++i ) {
		values1 = queue_dequeue( &queue1 );
		printf( "%d ", *values1 );
	}
	print_count_and_capacity();

	struct Test **values2;
	for ( int i = 0; i < 15; ++i ) {
		values2 = queue_dequeue( &queue2 );
		printf( "field1 == %d, field2 == %d, field3 == %d\n", (*values2)->field1, (*values2)->field2, (*values2)->field3 );
		free_struct_test( values2 );
	}
	putchar( '\n' );
	print_count_and_capacity();

	printf( "(3) Repopulating queue2\n" );
	populate_queue2( 5 );
	print_count_and_capacity();
	// Move `head` away from zero
	for ( int i = 0; i < 5; ++i ) {
		values2 = queue_dequeue( &queue2 );
		printf( "field1 == %d, field2 == %d, field3 == %d\n", (*values2)->field1, (*values2)->field2, (*values2)->field3 );
		free_struct_test( values2 );
	}
	printf( "(4) Checking count/capacity/head after populating and dequeing just a few elements from queue2: \n" );
	print_count_and_capacity();

	// Testing shrink_to_size
	printf( "(5) shrink to size, where new_size is greater than count but less than capacity:\n" );
	queue_shrink_to_size( 38, &queue2, free_struct_test );
	print_count_and_capacity();

	printf( "(6) shrink to size, where new_size is less than count but greater than head:\n" );
	queue_shrink_to_size( 25, &queue2, free_struct_test );
	print_count_and_capacity();

	printf( "(7) shrink to size, where new_size is less than head:\n" );
	queue_shrink_to_size( 2, &queue2, free_struct_test );
	print_count_and_capacity();

	printf( "Freeing the queues\n" );
	queue_free( &queue1 );
	queue_free_all_elements( &queue2, free_struct_test );
	queue_free( &queue2 );

	
	return 0;
	
}




