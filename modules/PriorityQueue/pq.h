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

// heap's minimum starting size
#define MIN_SIZE 16

typedef struct pq* PQueue;


// creates priority queue
// -requires a compare function
//           a destroy function (or NULL if you want to preserve the data)
PQueue pq_create(const CompareFunc, const DestroyFunc);

// inserts value at the priority queue
void pq_insert(const PQueue, const Pointer value);

// returns the element with the highest priority as given by the compare function
// or NULL if it's empty
// it's important to note that once removed, the element is not destroyed by the
// the destroy function (pq_destroy)
Pointer pq_remove(const PQueue);

// returns the size of the priority queue
uint64_t pq_size(const PQueue);

// returns true if the priority queue is empty, false otherwise
bool is_pq_empty(const PQueue);

// changes the destroy function and returns the old one
DestroyFunc pq_set_destroy(const PQueue, const DestroyFunc);

// destroys memory used by the priority queue
void pq_destroy(const PQueue);
