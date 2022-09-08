#pragma once  // include at most once

#include <stdbool.h>

typedef void* Pointer;

// Pointer to function that compares 2 elements a and b and returns:
// < 0  if a < b
//   0  if a and b are equal
// > 0  if a > b
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Pointer to function that destroys an element value
typedef void (*DestroyFunc)(Pointer value);

typedef struct queue* Queue;


// initializes queue
void queue_init(Queue*, DestroyFunc);

// enqueues value at the end of the queue
void queue_enqueue(Queue, Pointer value);

// sorted insert of value, as indicated by the compare function
void queue_sorted_insert(Queue, Pointer value, CompareFunc);

// dequeues value from the start of the queue and returns it, returns NULL if the queue is empty
Pointer queue_dequeue(Queue);

// returns the size of the queue
uint queue_size(Queue);

// returns true if the queue is empty, false otherwise
bool is_queue_empty(Queue);

// changes the destroy function and returns the old one
DestroyFunc queue_set_destroy(Queue, DestroyFunc);

// frees queue and its values if a destroy function is given
void queue_destroy(Queue);
