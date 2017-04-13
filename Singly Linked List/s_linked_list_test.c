#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// user defined type
typedef struct {
	
	int field1;
	int field2;
	int field3;

} List_Node_Data;

#include "s_linked_list.h"

// user defined function
bool
list_node_data_match( List_Node_Data *v1, List_Node_Data *v2 )
{
	bool match = false;

	if ( v1->field1 == v2->field1 ) {
		if ( v1->field2 == v2->field2 ) {
			if ( v1->field3 == v2->field3 ) {
				match = true;
			}
		}
	}
	
	return match;
}

void print_details( Linked_List *list )
{
	printf( "HEAD == %d\n", list->head->data.field1 );
	printf( "TAIL == %d\n", list->tail->data.field1 );
	printf( "COUNT == %zd\n\n", list->count );	

	Linked_List_Node *current;
	current = list->head;
	while ( current != NULL ) {
		printf( "Field1 == %d, Field2 == %d, Field3 == %d\n", current->data.field1, current->data.field2, current->data.field3 );
		current = current->next;
	}	
	putchar( '\n' );
	return;
}


int main( void ) {

	Linked_List list = linked_list_init();
	List_Node_Data value;

	printf( "(1)Populating:\n" );
	for (int i = 0; i < 5; ++i ) {
		value.field1 = i;
		value.field2 = i + 1;
		value.field3 = i + 2;

		linked_list_append( &value, &list );
	}

	print_details( &list );
	
	value.field1 = 5;
	value.field2 = 6;
	value.field3 = 7;

	printf( "(2)Prepending:\n" );
	linked_list_prepend( &value, &list);

	print_details( &list );
	
	List_Node_Data to_remove;
	List_Node_Data removed_value;
	bool removed;

	to_remove.field1 = 3;
	to_remove.field2 = 4;
	to_remove.field3 = 5;

	printf( "(3)Removing a value from the middle:\n" );
	removed_value = linked_list_remove( &to_remove, &list, list_node_data_match, &removed );

	print_details( &list );

	to_remove.field1 = 5;
	to_remove.field2 = 6;
	to_remove.field3 = 7;

	printf( "(4)Removing the head:\n" );
	removed_value = linked_list_remove( &to_remove, &list, list_node_data_match, &removed );	

	print_details( &list );

	to_remove.field1 = 4;
	to_remove.field2 = 5;
	to_remove.field3 = 6;

	printf( "(5)Removing the tail:\n" );
	removed_value = linked_list_remove( &to_remove, &list, list_node_data_match, &removed );	

	print_details( &list );

	printf( "(6)Reversing the list:\n" );
	linked_list_reverse( &list );

	print_details( &list );
	
	return 0;
}







