#pragma once  // include at most once

#include <stdbool.h>

typedef void* Pointer;

// Pointer to function that destroys an element value
typedef void (*DestroyFunc)(Pointer value);

typedef struct queue* Queue;


// initializes queue
void queue_init(Queue* Q, DestroyFunc destroy);

// enqueues value at the end of the queue
void queue_enqueue(Queue Q, Pointer value);

// dequeues value from the start of the queue and returns it
Pointer queue_dequeue(Queue Q);

// returns the size of the queue
uint queue_size(Queue Q);

// changes the destroy function and returns the old one
DestroyFunc queue_set_destroy(Queue Q, DestroyFunc new_destroy_func);

// frees queue and its values if a destroy function is given
void queue_destroy(Queue Q);
