#pragma once  // include at most once

#include <stdbool.h>
#include <stdint.h>


typedef void* Pointer;

// Pointer to function that compares 2 elements a and b and returns 0 if a and b are equal
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Pointer to function that destroys an element value
typedef void (*DestroyFunc)(Pointer value);

// Pointer to function that hashes a value to a positive (unsigned) integer
typedef unsigned int (*HashFunc)(Pointer value);

typedef struct hash_table* HashTable;


// creates hash table
// -requires a hash function
//           a compare function
//           a destroy function (or NULL if you want to preserve the data)
HashTable hash_create(const HashFunc, const CompareFunc, const DestroyFunc);

// inserts value at the hash table
// returns true if the value was inserted, false if it already exists
bool hash_insert(const HashTable, const Pointer);

// removes the value from the hash table and destroys its value if a destroy function was given
// returns true if the value was deleted, false in any other case
bool hash_remove(const HashTable, const Pointer);

// returns true if value exists in the hash table, false otherwise
bool hash_exists(const HashTable, const Pointer);

// returns the number of elements in the hash table
uint64_t hash_size(const HashTable);

// returns true if the hash table is empty, false otherwise
bool is_ht_empty(const HashTable);

// changes the destroy function and returns the old one
DestroyFunc hash_set_destroy(const HashTable, const DestroyFunc);

// destroys the memory used by the hash table
void hash_destroy(const HashTable);
