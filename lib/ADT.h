#pragma once  // include at most once

#include <stdbool.h>
#include <stdint.h>


/**********************************\
 ==================================
            DATA STRUCTURE
            TYPEDEFS
\**********************************/

// generic pointer
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
typedef uint32_t Vertex;
typedef uint32_t cost;  // weighted graph's edge cost

// Pointer to function that visits the vertices
typedef void (*VisitFunc)(Vertex V);




/**********************************\
 ==================================
            DATA STRUCTURE
            HANDLES
\**********************************/

typedef struct vector_struct* Vector;      // vector (Vector)
typedef struct StackSet* Stack;            // stack (Stack)
typedef struct queue* Queue;               // queue (Queue)
typedef struct pq* PQueue;                 // priority queue (PQueue)
typedef struct Set* RBTree;                // red-black tree (RBTree)
typedef struct hash_table* HashTable;      // hash table (HashTable)
typedef struct bfilter* bloom_filter;      // bloom filter (bloom_filter)
typedef struct _dir_graph* dir_graph;      // directed graph (dir_graph)
typedef struct _undir_graph* undir_graph;  // undirected graph (undir_graph)
typedef struct _wu_graph* wu_graph;        // weighted undirected (wu_graph)




/**********************************\
 ==================================
            DATA STRUCTURE
            UTILITIES
\**********************************/

// VECTOR
// -requires a destroy function
Vector vector_create(const DestroyFunc);                                    // creates vector
void vector_push_back(const Vector, const Pointer);                         // inserts the element at the back of the vector
Pointer vector_at(const Vector, const uint64_t);                            // returns the element at the given index
void vector_set_at(const Vector, const uint64_t, const Pointer);            // sets the value at the given index
bool vector_clear_at(const Vector, const uint64_t);                         // clear item at given vertex
uint64_t vector_size(const Vector);                                         // returns vector's size
bool is_vector_empty(const Vector);                                         // returns true if the vector is empty, false otherwise
bool vector_delete(const Vector, const Pointer, const CompareFunc);         // searches for the element and removes it
void vector_sort(const Vector, const CompareFunc);                          // sorts the vector using the compare function given
bool vector_binary_search(const Vector, const Pointer, const CompareFunc);  // searches the vector using binary search
bool vector_search(const Vector, const Pointer, const CompareFunc);         // searches the vector using linear search
DestroyFunc vector_set_destroy(const Vector, const DestroyFunc);            // changes the destroy function and returns the old one
void vector_destroy(const Vector);                                          // destroys the memory used by the vector


// STACK
// -requires a destroy function
Stack stack_create(const DestroyFunc);                          // creates stack
void stack_push(const Stack, const Pointer);                    // pushes value at the top of the stack
Pointer stack_pop(const Stack);                                 // pops value from the top of the stack
uint64_t stack_size(const Stack);                               // returns the size of the stack
bool is_stack_empty(const Stack);                               // returns true if the stack is empty, false otherwise
Pointer stack_top_value(const Stack);                           // returns the value at the top of the stack
DestroyFunc stack_set_destroy(const Stack, const DestroyFunc);  // changes the destroy function and returns the old one
void stack_destroy(const Stack);                                // destroys the memory used by the stack


// QUEUE
// -requires a destroy function
Queue queue_create(const DestroyFunc);                                    // creates queue
void queue_enqueue(const Queue, const Pointer);                           // enqueues value at the end of the queue
void queue_sorted_insert(const Queue, const Pointer, const CompareFunc);  // sorted insert of value
Pointer queue_dequeue(const Queue);                                       // dequeues value from the start of the queue
uint64_t queue_size(const Queue);                                         // returns the size of the queue
bool is_queue_empty(const Queue);                                         // returns true if the queue is empty, false otherwise
DestroyFunc queue_set_destroy(const Queue, const DestroyFunc);            // changes the destroy function and returns the old one
void queue_destroy(const Queue);                                          // destroys the memory used by the queue


// PRIORITY QUEUE
// -requires a compare and destroy function
PQueue pq_create(const CompareFunc, const DestroyFunc);       // creates priority queue
void pq_insert(const PQueue, const Pointer);                  // inserts value at the priority queue
Pointer pq_remove(const PQueue);                              // returns the element with the highest priority
uint64_t pq_size(const PQueue);                               // returns the size of the priority queue
bool is_pq_empty(const PQueue);                               // returns true if the priority queue is empty, false otherwise
Pointer pq_peek(const PQueue);                                // returns the element with the highest priority without removing it
DestroyFunc pq_set_destroy(const PQueue, const DestroyFunc);  // changes the destroy function and returns the old one
void pq_destroy(const PQueue);                                // destroys memory used by the priority queue


// RED-BLACK TREE
// -requires a compare and destroy function
typedef struct tnode* RBTreeNode;  // rbt node handle
RBTree rbt_create(const CompareFunc, const DestroyFunc);       // creates red-black tree
bool rbt_insert(const RBTree, const Pointer);                  // insert the item
bool rbt_remove(const RBTree, const Pointer);                  // remove the item
bool rbt_exists(const RBTree, const Pointer);                  // returns true if the value exists, false otherwise
uint64_t rbt_size(const RBTree);                               // returns the size of the tree
bool is_rbt_empty(const RBTree);                               // returns true if the tree is empty, false otherwise
DestroyFunc rbt_set_destroy(const RBTree, const DestroyFunc);  // changes the destroy function and returns the old one
void rbt_destroy(const RBTree);                                // destroys the memory used by the tree
Pointer rbt_node_value(const RBTreeNode);                      // returns the value of the node
RBTreeNode rbt_find_node(const RBTree, const Pointer);         // returns the node with that value, if it exists, otherwise NULL
RBTreeNode rbt_find_previous(const RBTreeNode);                // returns the previous, in order, node of target
RBTreeNode rbt_find_next(const RBTreeNode);                    // returns the next, in order, node of target
RBTreeNode rbt_first(const RBTree);                            // returns the node with the lowest value
RBTreeNode rbt_last(const RBTree);                             // returns the node with the highest value


// HASH TABLE
// -requires a hash, compare and destroy function
HashTable hash_create(const HashFunc, const CompareFunc, const DestroyFunc);  // creates hash table
bool hash_insert(const HashTable, const Pointer);                             // inserts value at the hash table
bool hash_remove(const HashTable, const Pointer);                             // removes the value from the hash table
bool hash_exists(const HashTable, const Pointer);                             // returns true if value exists in the hash table
uint64_t hash_size(const HashTable);                                          // returns the number of elements in the hash table
bool is_ht_empty(const HashTable);                                            // returns true if the hash table is empty, false otherwise
DestroyFunc hash_set_destroy(const HashTable, const DestroyFunc);             // changes the destroy function and returns the old one
void hash_destroy(const HashTable);                                           // destroys the memory used by the hash table

// provided hash functions
unsigned int hash_int1(Pointer);     // hashes an integer (1)
unsigned int hash_int2(Pointer);     // hashes an integer (2)
unsigned int hash_int3(Pointer);     // hashes an integer (3)
unsigned int hash_string1(Pointer);  // hashes a string (1)
unsigned int hash_string2(Pointer);  // hashes a string (2)
unsigned int hash_string3(Pointer);  // hashes a string (3)


// BLOOM FILTER
// -requires an array of hash functions
bloom_filter bf_create(const uint32_t, HashFunc*, const uint8_t);  // creates bloom filter (number of elements, hash functions, number of hash functions)
void bf_insert(const bloom_filter, const Pointer);                 // inserts value at the bloom filter
bool bf_exists(const bloom_filter, const Pointer);                 // returns true if value exists (possibly falsely) in the bloom filter, false otherwise
void bf_destroy(bloom_filter);                               // destroys the memory used by the bloom filter


// DIRECTED GRAPH
// -visit function required
dir_graph dg_create(const uint32_t, const VisitFunc);             // creates directed graph
void dg_insert(const dir_graph, const Vertex A, const Vertex B);  // inserts edge (A-B) at the graph
void dg_print(const dir_graph);                                   // prints the graph
void dg_dfs(const dir_graph);                                     // depth first traversal of the graph
dir_graph dg_reverse(const dir_graph);                            // returns reversed graph
void dg_bts(const dir_graph);                                     // prints a topological ordering of the graph
void dg_scc(const dir_graph);                                     // prints strongly-connected components
void dg_destroy(const dir_graph);                                 // destroys the memory used by the directed graph


// UNDIRECTED GRAPH
// -visit function required
undir_graph ug_create(const uint32_t, const VisitFunc);                      // creates undirected graph
void ug_insert(undir_graph, Vertex A, Vertex B);                             // inserts edge (A-B) at the graph
void ug_print(const undir_graph);                                            // prints the graph
void ug_simplepathcheck(const undir_graph, const Vertex S, const Vertex G);  // prints a "simple" path between vertices S and G, if such path exists
void ug_destroy(const undir_graph);                                          // destroys the memory used by the undirected graph


// WEIGHTED UNDIRECTED GRAPH
// -no functions required
wu_graph wug_create(const uint32_t);                                          // creates weighted undirected graph
void wug_insert(const wu_graph, const Vertex A, const Vertex B, const cost);  // inserts edge (A-B) with its weight at the graph
void wug_print(const wu_graph);                                               // prints the graph
void wug_minspantree(const wu_graph);                                         // prints minimum spanning tree, as well as its total weight
void wug_destroy(const wu_graph);                                             // destroys the memory used by the weighted undirected graph
