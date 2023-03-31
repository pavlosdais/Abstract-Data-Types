#pragma once  // include at most once

#include <stdbool.h>
#include <stdint.h>

typedef void* Pointer;

// Pointer to function that compares 2 elements a and b and returns:
// < 0  if a < b
//   0  if a and b are equal
// > 0  if a > b
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Pointer to function that destroys an element value
typedef void (*DestroyFunc)(Pointer value);

typedef struct queue* Queue;


// creates queue
// -requires a destroy function (or NULL if you want to preserve the data)
Queue queue_create(const DestroyFunc);

// enqueues value at the end of the queue
void queue_enqueue(const Queue, const Pointer);

// sorted insert of value, as indicated by the compare function
void queue_sorted_insert(const Queue, const Pointer, const CompareFunc);

// dequeues value from the start of the queue and returns it, returns NULL if the queue is empty
Pointer queue_dequeue(const Queue);

// returns the size of the queue
uint64_t queue_size(const Queue);

// returns true if the queue is empty, false otherwise
bool is_queue_empty(const Queue);

// changes the destroy function and returns the old one
DestroyFunc queue_set_destroy(const Queue, const DestroyFunc);

// destroys the memory used by the queue
void queue_destroy(const Queue);
