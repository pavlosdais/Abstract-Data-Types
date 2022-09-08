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

// Pointer to function that hashes a value to a positive integer - needed only by the hash table
typedef unsigned int (*HashFunc)(Pointer value);


// ADT handles
typedef struct StackSet* Stack;  // stack (Stack)
typedef struct queue* Queue;  // queue (Queue)
typedef struct pq* PQueue; // priority queue (PQueue)
typedef struct Set* RBTree;  // red-black tree (RBTree)
typedef struct hash_table* HashTable;  // hash table (HashTable)

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

// Stack
// -requires a destroy function
void stack_init(Stack*, DestroyFunc);
void stack_push(Stack, Pointer value);
Pointer stack_pop(Stack);
unsigned int stack_size(Stack);
bool is_stack_empty(Stack);
Pointer stack_top_value(Stack);
DestroyFunc stack_set_destroy(Stack, DestroyFunc);
void stack_destroy(Stack);


// Queue
// -requires a destroy function
void queue_init(Queue*, DestroyFunc);
void queue_enqueue(Queue, Pointer value);
void queue_sorted_insert(Queue, Pointer value, CompareFunc);
Pointer queue_dequeue(Queue);
unsigned int queue_size(Queue);
bool is_queue_empty(Queue);
DestroyFunc queue_set_destroy(Queue, DestroyFunc);
void queue_destroy(Queue);


// Priority Queue
// -requires a compare and destroy function
void pq_init(PQueue*, CompareFunc, DestroyFunc);
void pq_insert(PQueue, Pointer value);
Pointer pq_remove(PQueue);
unsigned int pq_size(PQueue);
bool is_pq_empty(PQueue);
DestroyFunc pq_set_destroy(PQueue, DestroyFunc);
void pq_destroy(PQueue);


// Red-Black Tree
// -requires a compare and destroy function
typedef struct tnode* RBTreeNode;  // node handle
void rbt_init(RBTree*, CompareFunc, DestroyFunc);
bool rbt_insert(RBTree, Pointer value);
bool rbt_remove(RBTree, Pointer value);
bool rbt_exists(RBTree, Pointer value);
unsigned int rbt_size(RBTree);
bool is_rbt_empty(RBTree);
DestroyFunc rbt_set_destroy(RBTree, DestroyFunc);
void rbt_destroy(RBTree);
Pointer rbt_node_value(RBTreeNode);
RBTreeNode rbt_find_node(RBTree, Pointer value);
RBTreeNode rbt_find_previous(RBTreeNode);
RBTreeNode rbt_find_next(RBTreeNode);
RBTreeNode rbt_first(RBTree);
RBTreeNode rbt_last(RBTree);


// Hash Table
// -requires a hash, compare and destroy function
void hash_init(HashTable*, HashFunc, CompareFunc, DestroyFunc);
bool hash_insert(HashTable, Pointer value);
bool hash_remove(HashTable, Pointer value);
bool hash_exists(HashTable, Pointer value);
bool is_ht_empty(HashTable);
unsigned int hash_size(HashTable);
DestroyFunc hash_set_destroy(HashTable, DestroyFunc);
void hash_destroy(HashTable);

// provided hash functions
unsigned int hash_int(Pointer value);  // hashes an integer

unsigned int hash_string1(Pointer value);  // hashes a string
unsigned int hash_string2(Pointer value);  // hashes a string
unsigned int hash_string3(Pointer value);  // hashes a string
