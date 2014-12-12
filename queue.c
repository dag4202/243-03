//
// Name: queue.c
// queue.c is the implementation using FIFO ordering. Resizes to hold 
// a dynamic amount of various data types. Uses a double pointer to hold
// the data objects.
//
// @author: dag4202: dyangelo a grullon
//
// git:
//	version: 0.1
//	path:  home/stu10/s4/dag4202/Courses/cs243v2/Projects/03/.git
// // // // // // // // // // // // // // // // // // // // // // // //

#include <stdlib.h>
#include <assert.h>


#define QUEUE_ALLOC_UNIT 10

struct queueStruct {
	void **content;
	unsigned int capacity;
	unsigned int end;
	unsigned int start;
};

typedef struct queueStruct *Queue;

#define _QUEUE_IMPL_
#include "queue.h"

//Initiates a que with initial values of 0
Queue que_create(void) {
	Queue queue;

	queue = (Queue) malloc(sizeof(struct queueStruct));
	queue->content = 0;
	queue->capacity = 0;
	queue->end = 0;
	queue->start = 0;
	return queue;
}

//Frees all allocated memory in the queue
void que_destroy(Queue queue){
	assert(queue != 0);
	if(queue->content!=0) {
		free(queue->content);
	}

	free( queue );
}

//Removes all data from the queue
void que_clear( Queue queue ){
	if (queue->content != 0) {
		free(queue->content);
		queue->content = 0;
	}
	queue->capacity = 0;
	queue->end = 0;
	queue->start = 0;
}

//Add data to the queue. Dynamically allocates if the queue is full.
void que_push(void *data, Queue queue){
	assert(queue!=0);
	if (queue->content==0){
		queue->content = malloc(sizeof(void *) * QUEUE_ALLOC_UNIT);
		queue->capacity = QUEUE_ALLOC_UNIT;
	}
	if (queue->end >= queue->capacity){
		void *tmp;
		if (queue->start!=0){
			void **tempQueue = malloc(sizeof(void *) * queue->capacity);
			int count= 0;
			for (unsigned int i = queue->start; i< queue->end; i++){
				tmp = queue->content[i];
				tempQueue[count] = queue->content[i];
				count+=1;
			}
			free(queue->content);
			queue->content = tempQueue;
			queue->end -=queue->start;
			queue->start = 0;
		} else {
			tmp = realloc(queue->content, (queue->capacity + 
				QUEUE_ALLOC_UNIT) * sizeof(void *));
			queue->content = tmp;
			queue->capacity += QUEUE_ALLOC_UNIT;
		}
	}
	queue->content[queue->end] = data;
	queue->end += 1;
	
}

// Remove and Return the data in the front of the queue.
void *que_pop( Queue queue ) {
	assert((queue->end > 0) && (queue->start < queue->end));
	
	void *data = queue->content[queue->start++];
	if (queue->start == queue->end){
		queue->start=0;
		queue->end = 0;
		return data;
	}
	return data;
}

//Return the front most data object.
void *que_front(Queue queue) {
	assert((queue->end > 0) && (queue->start < queue->end));
	
	return queue->content[queue->start];

}

//Returns true if the queue is empty, and false otherwise
int que_isEmpty(Queue queue){
	return (queue->start == queue->end);
}

//Gets the data in the queue based on pos, which is used
//as an index relative to the front of the queue.
void *que_get( unsigned int pos, Queue queue){
	assert((pos+queue->start) < queue->end);

	return queue->content[queue->start + pos];
	
}

//Returns the size of the queue.
unsigned int que_size(Queue queue){
	return (queue->end - queue->start);
}
