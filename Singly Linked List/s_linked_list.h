#ifndef S_LINKED_LIST_H_
#define S_LINKED_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*
 * 	The idea of this implemenation wat to create a generic singly linked list without the use of void *.
 * 	It's based off the idea of an intrusive data structure, where the data is directly embedded within the
 * 	container itself. This allows the user to embed whatever data they want on the list in a 
 * 	`List_Node_Data` and the implementation just takes care of it.
 *
 * 	Limitation -- can't define two types of generic lists in the same compilation unit because the linker
 *               wouldn't be able to resolve the conflicting symbols, or it may just silently replace one
 *               of them...no good.
 */

/**** Library Usage Requirements ***/ 
/*
 * 	User must define a type called `List_Node_Data`.
 *  The contest of this structure will be whatever you want contained on the list.
 *
 *	IMPORTANT: The definition must come BEFORE the #include directive for s_linked_list.h.
 *
 * 	Ex.
 * 	List_Node_Data {
 *		int value;			// hold simple int values
 *  };						// note: sizeof ( List_Node_Data) == sizeof (int)
 *
 *  	OR
 *
 * 	struct Super_Big_Struct {
 *		...(n number of fields)
 *	};
 *
 *	 List_Node_Data {
 * 		struct Super_Big_Struct data;  // could embed directly
 *	};
 *
 *  	OR
 *
 *	List_Node_Data {
 *		struct Super_Big_Struct *data; // hold a pointer to heap allocated struct Super_Big_Struct
 *
 * 
 *	User must also define a match function to test for equality. This is used in
 *  linked_list_contains() and linked_list_remove(). The defintion is depenedent
 *  on how equality is defined on your embedded types.
*/

/* Finally, i have named the header file s_linked_list.h even though it contains 
 * the source as well. This has to be done to satisfy the compiler. If the files
 * were separated into .h and .c then the compiler would be unable to find the 
 * storage space required for the List_Node_Data.
 *
*/

typedef struct Linked_List_Node {
	
	List_Node_Data data;
	struct Linked_List_Node *next;
	
} Linked_List_Node;

typedef struct {

	Linked_List_Node *head;
	Linked_List_Node *tail;
	size_t count;

} Linked_List;

Linked_List
linked_list_init( void ) 
{
	Linked_List list;

	list.head = list.tail = NULL;

	return list;
}

static Linked_List_Node *
allocate_linked_list_node( void *element )
{
	Linked_List_Node *new_node; 
	
	new_node = (Linked_List_Node *)malloc( sizeof *new_node );
    if ( !new_node ) {
    	fprintf( stderr, "allocate_linked_list_node()...unable to allocate a new list node\n" );
    	exit( EXIT_FAILURE );
    }
          
    memcpy( &(new_node->data), element, sizeof (List_Node_Data) );
    new_node->next = NULL; 
    
    return new_node;
}

void
linked_list_prepend( void *element, Linked_List *list )
{
	Linked_List_Node *new_node;

	new_node = allocate_linked_list_node( element );

	if ( list->head == NULL ) {
		list->head = new_node;
		list->tail = new_node;
	}
	else {
		new_node->next = list->head;
		list->head = new_node;
	}

	list->count++;

	return;
}

void
linked_list_append( void *element, Linked_List *list )
{
	Linked_List_Node *new_node;

	new_node = allocate_linked_list_node( element );

	if ( list->head == NULL ) {
		list->head = new_node;
		list->tail = new_node;

		return;
	}
	else {
		list->tail->next = new_node;
		list->tail = new_node;
	}

	list->count++;

	return;
}

// returns the  Linked_List_Data of the node being removed in case it contains heap allocated data
//  that needs to be freed by the user
//  Linked_List_Data `retval` is unitialized and will return garbage if nothing was removed
// check `removed` parameter in caller to see if something was removed
List_Node_Data
linked_list_remove( List_Node_Data *key, 
					Linked_List *list, 
					bool(*match_fn)( List_Node_Data *, List_Node_Data *),
					bool *removed )
{
	Linked_List_Node *previous;
	Linked_List_Node *current;
	List_Node_Data retval;

	// reset removed in case someone reuses it from a previously successful remove
	*removed = false;

	current = list->head;
	if ( match_fn( key, &current->data ) == true ) {
		*removed = true;
		retval = current->data;
		list->count--;

		if ( current->next == NULL ) {
			free( current );
			list->head = NULL;
			list->tail = NULL;
		}
		else {
			list->head = current->next;
			free( current );
		}

		return retval;
		
	}

	previous = current;
	current = current->next;
	while ( current != NULL ) {
		if ( match_fn( key, &(current->data) ) == 1 ) {
			*removed = true;
			list->count--;
			retval = current->data;	
			previous->next = current->next;		

			if ( current->next == NULL ) {
				list->tail = previous;
			}

			free( current );
			break;
		}

		previous = current;
		current = current->next;
	}

	return retval;
}

void
linked_list_reverse( Linked_List *list ) 
{
	Linked_List_Node *previous;
	Linked_List_Node *current;
	Linked_List_Node *next;

	previous = NULL;
	current = list->head;
	if ( current->next == NULL ) {
		return;
	}

	while ( current != NULL ) {
		next = current->next;
		current->next = previous;
		previous = current;
		current = next;
	}

	 Linked_List_Node *temp;

	temp = list->head;
	list->head = list->tail;
	list->tail = temp;

	return;
}

Linked_List_Node *
linked_list_contains( List_Node_Data *key,
					  Linked_List *list,
					  bool(*match_fn)(List_Node_Data *, List_Node_Data *) )
{
	Linked_List_Node *current;

	current = list->head;
	while ( current != NULL ) {
		if ( match_fn( key, &(current->data) ) == true ) {
			break;
		}

		current = current->next;
	}

	return current;
}

#endif






