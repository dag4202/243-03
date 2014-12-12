//
// Name: queue.h
// queue.h is a Queue ADT using FIFO ordering. Resizes to hold a dynamic 
// amount of various data types.
//
// @author: dag4202: dyangelo a grullon
//
// git:
//	version: 0.3
//	path:  home/stu10/s4/dag4202/Courses/cs243v2/Projects/03/.git
// // // // // // // // // // // // // // // // // // // // // // // //

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
* Removes and returns the first node in the queue.
*/
void *que_pop(Queue queue);

/**
* Returns the first node in the queue.
*/
void *que_front(Queue queue);

/**
* Returns 1 if the queue is empty, and 0 otherwise.
*/
int que_isEmpty(Queue queue);

/**
* Gets the data at the desired position in the queue relative to the
* front of the queue. 
*/
void *que_get( unsigned int pos, Queue queue);

/**
* Returns the number of objects held by the given queue.
*/
unsigned int que_size(Queue queue);
#endif
