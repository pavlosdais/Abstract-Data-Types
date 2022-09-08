#pragma once  // include at most once

#include <stdbool.h>

typedef struct hash_table* HashTable;

typedef void* Pointer;

// Pointer to function that compares 2 elements a and b and returns 0 if a and b are equal
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Pointer to function that destroys an element value
typedef void (*DestroyFunc)(Pointer value);

// Pointer to function that hashes a value to a positive (unsigned) integer
typedef uint (*HashFunc)(Pointer value);


// initializes hash table
void hash_init(HashTable*, HashFunc, CompareFunc, DestroyFunc);

// inserts value at the hash table
// returns true if the value was inserted, false if it already exists
bool hash_insert(HashTable, Pointer value);

// removes the value from the hash table and destroys its value if a destroy function was given
// returns true if the value was deleted, false in any other case
bool hash_remove(HashTable, Pointer value);

// returns true if value exists in the hash table, false otherwise
bool hash_exists(HashTable, Pointer value);

// returns the number of elements in the hash table
uint hash_size(HashTable);

// returns true if the hash table is empty, false otherwise
bool is_ht_empty(HashTable);

// changes the destroy function and returns the old one
DestroyFunc hash_set_destroy(HashTable, DestroyFunc);

// destroys the hash table and its values, if a destroy function is given
void hash_destroy(HashTable);
