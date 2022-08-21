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
void stack_init(Stack* S, DestroyFunc destroy);
void stack_push(Stack S, Pointer value);
Pointer stack_pop(Stack S);
unsigned int stack_size(Stack S);
Pointer stack_top_value(Stack S);
DestroyFunc stack_set_destroy(Stack S, DestroyFunc new_destroy_func);
void stack_destroy(Stack S);


// Queue
// -requires a destroy function
void queue_init(Queue* Q, DestroyFunc destroy);
void queue_enqueue(Queue Q, Pointer value);
Pointer queue_dequeue(Queue Q);
unsigned int queue_size(Queue Q);
DestroyFunc queue_set_destroy(Queue Q, DestroyFunc new_destroy_func);
void queue_destroy(Queue Q);


// Priority Queue
// -requires a compare and destroy function
void pq_init(PQueue* PQ, CompareFunc compare, DestroyFunc destroy);
void pq_insert(PQueue PQ, Pointer value);
Pointer pq_remove(PQueue PQ);
unsigned int pq_size(PQueue PQ);
DestroyFunc pq_set_destroy(PQueue PQ, DestroyFunc new_destroy_func);
void pq_destroy(PQueue PQ);


// Red-Black Tree
// -requires a compare and destroy function
typedef struct tnode* RBTreeNode;  // node handle
void rbt_init(RBTree* Tree, CompareFunc compare, DestroyFunc destroy);
bool rbt_insert(RBTree Tree, Pointer value);
bool rbt_remove(RBTree Tree, Pointer value);
bool rbt_exists(RBTree Tree, Pointer value);
unsigned int rbt_size(RBTree Tree);
DestroyFunc rbt_set_destroy(RBTree Tree, DestroyFunc new_destroy_func);
void rbt_destroy(RBTree Tree);
Pointer rbt_node_value(RBTreeNode rbt_node);
RBTreeNode rbt_find_node(RBTree Tree, Pointer value);
RBTreeNode rbt_find_previous(RBTreeNode target);
RBTreeNode rbt_find_next(RBTreeNode target);
RBTreeNode rbt_first(RBTree Tree);
RBTreeNode rbt_last(RBTree Tree);


// Hash Table
// -requires a hash, compare and destroy function
void hash_init(HashTable* ht, HashFunc hash, CompareFunc compare, DestroyFunc destroy);
bool hash_insert(HashTable ht, Pointer value);
bool hash_remove(HashTable ht, Pointer value);
bool hash_exists(HashTable ht, Pointer value);
unsigned int hash_size(HashTable ht);
DestroyFunc hash_set_destroy(HashTable ht, DestroyFunc new_destroy_func);
void hash_destroy(HashTable ht);

unsigned int hash_int(Pointer value);  // hashes an integer
unsigned int hash_string(Pointer value);  // hashes a string
