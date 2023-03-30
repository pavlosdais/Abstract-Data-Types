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

// Pointer to function that hashes a value to a positive integer - needed only by the hash table
typedef unsigned int (*HashFunc)(Pointer value);


// Graph typedefs
typedef int Vertex;
typedef uint32_t cost;  // weighted graph's edge cost

// Pointer to function that visits the vertices
typedef void (*VisitFunc)(Vertex V);


// ADT handles
typedef struct vector_struct* Vector;  // vector (Vector)
typedef struct StackSet* Stack;  // stack (Stack)
typedef struct queue* Queue;  // queue (Queue)
typedef struct pq* PQueue; // priority queue (PQueue)
typedef struct Set* RBTree;  // red-black tree (RBTree)
typedef struct hash_table* HashTable;  // hash table (HashTable)
typedef struct _dir_graph* dir_graph;  // directed graph (dir_graph)
typedef struct _undir_graph* undir_graph;  // undirected graph (undir_graph)
typedef struct _wu_graph* wu_graph;  // weighted undirected (wu_graph)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Vector
// -requires a destroy function
Vector vector_create(const DestroyFunc);
uint64_t vector_size(const Vector);
Pointer vector_at(const Vector, const uint64_t);
void vector_set_at(const Vector, const uint64_t, const Pointer);
void vector_push_back(const Vector, const Pointer);
bool vector_clear_at(const Vector, const uint64_t);
void vector_sort(const Vector, const CompareFunc);
bool vector_binary_search(const Vector vector, const Pointer, const CompareFunc);
bool vector_search(const Vector, const Pointer, const CompareFunc);
void vector_destroy(const Vector);


// Stack
// -requires a destroy function
Stack stack_create(const DestroyFunc);
void stack_push(const Stack, const Pointer value);
Pointer stack_pop(const Stack);
uint64_t stack_size(const Stack);
bool is_stack_empty(const Stack);
Pointer stack_top_value(const Stack);
DestroyFunc stack_set_destroy(const Stack, const DestroyFunc);
void stack_destroy(const Stack);


// Queue
// -requires a destroy function
Queue queue_create(const DestroyFunc);
void queue_enqueue(const Queue, const Pointer);
void queue_sorted_insert(const Queue, const Pointer, CompareFunc);
Pointer queue_dequeue(Queue);
uint64_t queue_size(const Queue);
bool is_queue_empty(const Queue);
DestroyFunc queue_set_destroy(const Queue, DestroyFunc);
void queue_destroy(const Queue);


// Priority Queue
// -requires a compare and destroy function
PQueue pq_create(const CompareFunc, const DestroyFunc);
void pq_insert(const PQueue, const Pointer value);
Pointer pq_remove(const PQueue);
uint64_t pq_size(const PQueue);
bool is_pq_empty(const PQueue);
DestroyFunc pq_set_destroy(const PQueue, const DestroyFunc);
void pq_destroy(const PQueue);


// Red-Black Tree
// -requires a compare and destroy function
typedef struct tnode* RBTreeNode;  // node handle
RBTree rbt_create(const CompareFunc, const DestroyFunc);
bool rbt_insert(const RBTree, const Pointer value);
bool rbt_remove(const RBTree, const Pointer value);
bool rbt_exists(const RBTree, const Pointer value);
uint64_t rbt_size(const RBTree);
bool is_rbt_empty(const RBTree);
DestroyFunc rbt_set_destroy(const RBTree, const DestroyFunc);
void rbt_destroy(const RBTree);
Pointer rbt_node_value(const RBTreeNode);
RBTreeNode rbt_find_node(const RBTree, const Pointer value);
RBTreeNode rbt_find_previous(const RBTreeNode);
RBTreeNode rbt_find_next(const RBTreeNode);
RBTreeNode rbt_first(const RBTree);
RBTreeNode rbt_last(const RBTree);


// Hash Table
// -requires a hash, compare and destroy function
HashTable hash_create(const HashFunc, const CompareFunc, const DestroyFunc);
bool hash_insert(const HashTable, const Pointer value);
bool hash_remove(const HashTable, const Pointer value);
bool hash_exists(const HashTable, const Pointer value);
uint64_t hash_size(const HashTable);
bool is_ht_empty(const HashTable);
DestroyFunc hash_set_destroy(const HashTable, const DestroyFunc);
void hash_destroy(const HashTable);

// provided hash functions
unsigned int hash_int(Pointer value);      // hashes an integer
unsigned int hash_string1(Pointer value);  // hashes a string
unsigned int hash_string2(Pointer value);  // hashes a string
unsigned int hash_string3(Pointer value);  // hashes a string


// Directed Graph
// -visit function required
dir_graph dg_create(const uint32_t, const VisitFunc);
void dg_insert(const dir_graph, const Vertex A, const Vertex B);
void dg_print(const dir_graph);
void dg_dfs(const dir_graph);
dir_graph dg_reverse(const dir_graph);
void dg_bts(const dir_graph);
void dg_scc(const dir_graph);
void dg_destroy(const dir_graph);


// Undirected Graph
// -visit function required
undir_graph ug_create(const uint32_t, const VisitFunc);
void ug_insert(undir_graph, Vertex, Vertex);
void ug_print(const undir_graph);
void ug_simplepathcheck(const undir_graph, const Vertex, const Vertex);
void ug_destroy(const undir_graph);


// Weighted Undirected Graph
// -no functions required
wu_graph wug_create(const uint32_t);
void wug_insert(const wu_graph, const Vertex A, const Vertex B, const cost);
void wug_print(const wu_graph);
void wug_minspantree(const wu_graph);
void wug_destroy(const wu_graph);
