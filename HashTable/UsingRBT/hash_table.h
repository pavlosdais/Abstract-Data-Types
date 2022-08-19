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


// number of buckets used
#define NUM_OF_BUCKETS 1000

// initializes hash table
void hash_init(HashTable* ht, HashFunc hash, CompareFunc compare, DestroyFunc destroy);

// inserts value at the hash table
// returns true if the value was inserted, false if it already exists
bool hash_insert(HashTable ht, Pointer value);

// removes the value from the hash table and destroys its value if a destroy function was given
// returns true if the value was deleted, false in any other case
bool hash_remove(HashTable ht, Pointer value);

// returns true if value exists in the hash table, false otherwise
bool hash_exists(HashTable ht, Pointer value);

// returns the number of elements in the hash table
uint hash_size(HashTable ht);

// changes the destroy function and returns the old one
DestroyFunc hash_set_destroy(HashTable ht, DestroyFunc new_destroy_func);

// destroys the hash table and its values, if a destroy function is given
void hash_destroy(HashTable ht);
