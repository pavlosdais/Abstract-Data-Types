#pragma once  // include at most once

#include <stdbool.h>

// heap's minimum starting size
#define MIN_SIZE 64

typedef void* Pointer;

typedef struct pq* PQueue;

// Pointer to function that compares 2 elements a and b and returns:
// < 0  if a < b
//   0  if a and b are equal
// > 0  if a > b
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Pointer to function that destroys an element value
typedef void (*DestroyFunc)(Pointer value);


// initializes priority queue
void pq_init(PQueue* PQ, CompareFunc compare, DestroyFunc destroy);

// inserts value at the priority queue
void pq_insert(PQueue PQ, Pointer value);

// returns the element with the highest priority as given by the compare function
// or NULL if it's empty
// it's important to note that once removed, the element is not destroyed by the
// the destroy function (pq_destroy)
Pointer pq_remove(PQueue PQ);

// returns the size of the priority queue
uint pq_size(PQueue PQ);

// changes the destroy function and returns the old one
DestroyFunc pq_set_destroy(PQueue PQ, DestroyFunc new_destroy_func);

// destroys memory used by the priority queue
void pq_destroy(PQueue PQ);
