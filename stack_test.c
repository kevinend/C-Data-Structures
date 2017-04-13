#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "stack.h"

Stack stack1;  // copy types
Stack stack2;  // owning pointers

struct Test {
	int field1;
	int field2;
	int field3;
};

void print_count_and_capacity( void )
{
	printf( "stack1 count == %zd, capacity == %zd\n", stack_count( &stack1 ), stack_capacity( &stack1 ) );
	printf( "stack2 count == %zd, capacity == %zd\n", stack_count( &stack2 ), stack_capacity( &stack2 ) );
	putchar( '\n' );

	return;
}

void free_struct_test( void *element ) 
{
	struct Test **value = (struct Test **)element;
	
	free( *value );
	return;
}

int main( void ) {

	stack1 = stack_init( sizeof (int) );
	stack2 = stack_init_with_capacity( 10, sizeof (struct Test *) );

	printf( "Baseline count and capacity: \n" );
	print_count_and_capacity();

	// Populate the non-owning stack with integers
	for ( int i = 0; i < 10; ++i ) {
		stack_push( &i, &stack1 );
	}
	
	// Populate the owning stack with pointers to heap allocated struct Test
	// I ignore checking the return value for malloc for this test
	struct Test *leaky;
	for ( int i = 0; i < 5; ++i ) {
		leaky = (struct Test *)malloc( 1 * sizeof *leaky );
		leaky->field1 = i;
		leaky->field2 = i + 1;
		leaky->field3 = i + 2;

		stack_push( &leaky, &stack2 );
	}

	print_count_and_capacity();

	// Pop the values off and print them 
	int *values;
	while ( !stack_is_empty( &stack1 ) ) {
		values = stack_pop( &stack1 );
		printf( "%d ", *values );
	}
	printf( "\n\n" );


	struct Test **tester;
	while ( !stack_is_empty( &stack2 ) ) {
		tester = stack_pop( &stack2 );
		printf( "Field1 == %d, Field2 == %d, Field3 == %d\n", (*tester)->field1, (*tester)->field2, (*tester)->field3 );
		free_struct_test( tester );
	}
	putchar( '\n' );

	// Repopulate the stacks
	for ( int i = 0; i < 5; ++i ) {
		stack_push( &i, &stack1 );
	}

	for ( int i = 0; i < 10; ++i ) {
		leaky = (struct Test *)malloc( 1 * sizeof *leaky );
		leaky->field1 = i;
		leaky->field2 = i + 1;
		leaky->field3 = i + 2;
	
		stack_push( &leaky, &stack2 );
	}
	print_count_and_capacity();

	printf( "Testing stack_clear: \n" );
	stack_clear( &stack2, free_struct_test );
	print_count_and_capacity();

	// Repopulate stack2
	for ( int i = 0; i < 20; ++i ) {
		leaky = (struct Test *)malloc( 1 * sizeof *leaky );
		leaky->field1 = i;
		leaky->field2 = i + 1;
		leaky->field3 = i + 2;

		stack_push( &leaky, &stack2 );
	}

	// Testing the shrink functions
	printf( "Shrinking both stacks: \n" );
	stack_shrink_to_fit( &stack1 );
	stack_shrink_to_size( 2, &stack2, free_struct_test );
	print_count_and_capacity();
	

	stack_free( &stack1 );
	stack_free_all_elements( &stack2, free_struct_test );
	stack_free( &stack2 );

	return 0;
}
