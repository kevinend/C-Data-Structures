#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "queue.h"

Queue
queue_init( size_t element_size ) 
{
	Queue queue;

	queue.data = darray_init( element_size );
	queue.head = 0;

	return queue;
}

Queue
queue_init_with_capacity( size_t initial_capacity, size_t element_size )
{
	Queue queue;

	queue.data = darray_init_with_capacity( initial_capacity, element_size );
	queue.head = 0;

	return queue;
}

bool 
queue_is_full( Queue *queue )
{
	return queue->data.count == queue->data.capacity;
}

/* used memmove since they are in overlapping regions of memory */
static void
queue_overwrite_dequeued_elements( Queue *queue ) 
{
	void *dequeued_elements;
	void *queued_elements;
	size_t queued_element_size;

	dequeued_elements = queue->data.buffer;
	queued_elements = (uint8_t *)queue->data.buffer + (queue->head * queue->data.element_size);

	queued_element_size = (queue->data.count - queue->head) * queue->data.element_size;
	memmove( dequeued_elements, queued_elements, queued_element_size );

	return;
}

void
queue_enqueue( void *element, Queue *queue ) 
{
	if ( queue_is_full( queue ) && queue->head != 0 ) {
		queue_overwrite_dequeued_elements( queue );
		queue->data.count = queue->data.count - queue->head;
		queue->head = 0;
	}

	darray_add( element, &(queue->data) );
	return;
}

void *
queue_dequeue( Queue *queue ) 
{
	void *dequeued_element;

	dequeued_element = darray_at_index( queue->head, &(queue->data) );
	queue->head++;

	return dequeued_element;
}

void 
queue_shrink_to_fit( Queue *queue ) 
{
	darray_shrink_to_fit( &(queue->data) );
	queue->head = queue->data.count;
	return;
}

/* Behavior different from darray_shrink_to_size */
/* Need to account for `head` field of the queue */
void
queue_shrink_to_size( size_t new_size, Queue *queue, void(*freefn)(void *) )
{
	if ( new_size < 0 || new_size >= queue->data.capacity ) {
		return;
	}

	if ( new_size < queue->head ) {
		if ( freefn != NULL ) {
			for ( size_t i = queue->head; i < queue->data.count; ++i ) {
				freefn( darray_at_index( i, &(queue->data) ) );
			}
		}

		queue->head  = new_size;
		queue->data.count = new_size;
	}
	else if ( new_size < queue->data.count ) {
		if ( freefn != NULL ) {
			for ( size_t i = new_size; i < queue->data.count; ++i ) {
				freefn( darray_at_index( i, &(queue->data) ) );
			}
		}

		queue->data.count = new_size;
	}


	queue->data.buffer = realloc( queue->data.buffer, queue->data.element_size * new_size );
	queue->data.capacity = new_size;

	return;
}

void
queue_free_all_elements( Queue *queue, void(*freefn)(void *) )
{
	darray_free_all_elements( queue->head, &(queue->data), freefn );
	return;
}

void 
queue_free( Queue *queue) 
{
	darray_free( &(queue->data) );
	return;
}

size_t
queue_count( Queue *queue )
{
	return queue->data.count;
}

size_t
queue_capacity( Queue *queue )
{
	return queue->data.capacity;
}







