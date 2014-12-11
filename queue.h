/**
* queue.h
* Dyangelo Grullon
* 
* Queue
*
* A Queue ADT using FIFO data management, multiple data types, and resizing 
* capabilities
*/

#ifndef _QUEUE_H_
#define _QUEUE_H_


#ifndef _QUEUE_IMPL_

typedef struct { } *Queue;

#endif


/**
* The init method for the Queue ADT. Creates an empty queue. Initializes values
* to 0.
*/
Queue que_create(void);

/**
* Frees all memory for any allocated component of the queue.
*/
void que_destroy(Queue queue); 

/**
* Removes all elements from the queue. Initializes elements to 0.
*/
void que_clear(Queue queue);

/**
* Adds generic data to the end of the queue. Resizes Dynamically.
*/
void que_push(void *data, Queue queue);

/**
* Removes and returnsthe first node in the queu
*/
void *que_pop(Queue queue);

void *que_front(Queue queue);

int que_isEmpty(Queue queue);

void *que_get( unsigned int pos, Queue queue);

unsigned int que_size(Queue queue);
#endif
