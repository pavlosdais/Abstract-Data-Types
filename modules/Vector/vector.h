#pragma once

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

typedef struct vector_struct* Vector;


// creates vector
// -requires a destroy function (or NULL if you want to preserve the data)
Vector vector_create(const DestroyFunc);

// inserts the element at the back of the vector
void vector_push_back(const Vector, const Pointer);

// sets the element at the given index (if there is an element there, it destroys it provided that a destroy function was given)
void vector_set_at(const Vector, const uint64_t, const Pointer);

// returns the element at the given index (NULL if no element exists there)
Pointer vector_at(const Vector, const uint64_t);

// if the an element exists at the given index, it clears it and returns true (also destroys it provided that a destroy function was given)
// otherwise returns false
bool vector_clear_at(const Vector, const uint64_t);

// returns the number of elements the vector currently stores
uint64_t vector_size(const Vector);

// returns true if the priority queue is empty, false otherwise
bool is_vector_empty(const Vector);

// changes the destroy function and returns the old one
DestroyFunc vector_set_destroy(const Vector, const DestroyFunc);

// searches the vector using linear search
// returns true if found, false if not
bool vector_search(const Vector, const Pointer, const CompareFunc);

// sorts the vector using the compare function given
void vector_sort(const Vector, const CompareFunc);

// searches the vector using binary search (requires the vector to be sorted, unidentified behaviour if not)
// returns true if found, false if not
bool vector_binary_search(const Vector, const Pointer, const CompareFunc);

// destroys memory used by the vector
void vector_destroy(const Vector);
