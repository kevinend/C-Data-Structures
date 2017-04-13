#ifndef QUEUE_H_
#define QUEUE_H_

#include "dynamic_array.h"

/* Queue is built using a dynamic array 
 *  `count` field of dynamic array marks the last element of the queue
 *  `capacity` defines the total size of the queue 
 *  additional field `head` marks the next element to be dequeued
*/

typedef struct {
	Dynamic_Array data;
	size_t head;
} Queue;

Queue queue_init( size_t ); 

Queue queue_init_with_capacity( size_t, size_t );

void queue_enqueue( void *, Queue * );

void * queue_dequeue( Queue * ); 

void queue_shrink_to_fit( Queue * );

void queue_shrink_to_size( size_t, Queue *, void(*)(void *) );

void queue_free_all_elements( Queue *, void(*)(void *) );

void queue_free( Queue * );

size_t queue_count( Queue * );

size_t queue_capacity( Queue * );

#endif




