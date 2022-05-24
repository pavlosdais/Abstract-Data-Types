#include <stdbool.h>

// Recommended sizes:
// 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241
// 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611,
// 402653189, 805306457, 1610612741, 3221225479

#define HASH_CAPACITY 53  // bucket size

typedef struct hash_table* HashTable;

typedef void* Pointer;

// Pointer to function that compares 2 elements a and b and returns:
// < 0  if a < b
//   0  if a and b are equal
// > 0  if a > b
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Pointer to function that destroys an element value
typedef void (*DestroyFunc)(Pointer value);

// Pointer to function that hashes a value to an positive integer
typedef unsigned int (*HashFunc)(Pointer value);

// initializes hash table
void hash_init(HashTable* ht, HashFunc hash, CompareFunc compare, DestroyFunc destroy);

// inserts value at the hash table
void hash_insert(HashTable ht, Pointer value);

// removes the value from the hash table and destroys its value if a destroy function was given
// returns true if the value was deleted, false in any other case
bool hash_remove(HashTable ht, Pointer value);

// returns true if value exists in the hash table, false otherwise
bool hash_exists(HashTable ht, Pointer value);

// change destroy function and return the old one
DestroyFunc hash_set_destroy(HashTable ht, DestroyFunc new_destroy_func);

// return the number of elements inserted
unsigned int hash_size(HashTable ht);

// destroys the hash table and its values, if a destroy function is given
void hash_destroy(HashTable ht);
