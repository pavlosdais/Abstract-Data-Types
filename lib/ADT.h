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
void stack_push(Stack, Pointer value);
Pointer stack_pop(Stack);
uint64_t stack_size(Stack);
bool is_stack_empty(Stack);
Pointer stack_top_value(Stack);
DestroyFunc stack_set_destroy(Stack, DestroyFunc);
void stack_destroy(Stack);


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
void dg_init(dir_graph*, uint32_t num_of_vertices, VisitFunc visit);
void dg_insert(dir_graph, Vertex A, Vertex B);
void dg_print(dir_graph);
void dg_dfs(dir_graph);
dir_graph dg_reverse(dir_graph);
void dg_bts(dir_graph);
void dg_scc(dir_graph);
void dg_destroy(dir_graph);


// Undirected Graph
// -visit function required
void ug_init(undir_graph* G, uint32_t num_of_vertices, VisitFunc visit);
void ug_insert(undir_graph G, Vertex A, Vertex B);
void ug_print(undir_graph A);
void ug_simplepathcheck(undir_graph G, Vertex start, Vertex goal);
void ug_destroy(undir_graph G);


// Weighted Undirected Graph
// -no functions required
void wug_init(wu_graph*, uint32_t num_of_vertices);
void wug_insert(wu_graph, Vertex A, Vertex B, cost);
void wug_print(wu_graph);
void wug_minspantree(wu_graph);
void DestroyGraph(wu_graph);
