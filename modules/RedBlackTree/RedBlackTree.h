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

typedef struct Set* RBTree;


// creates red-black tree
// -requires a compare function
//           a destroy function (or NULL if you want to preserve the data)
RBTree rbt_create(const CompareFunc, const DestroyFunc);

// returns true if the item is inserted, in any other case false
bool rbt_insert(const RBTree, const Pointer);

// returns true if the item is deleted, in any other case false
bool rbt_remove(const RBTree, const Pointer);

// returns true if the value exists, false otherwise
bool rbt_exists(const RBTree, const Pointer);

// returns the size of the tree
uint64_t rbt_size(const RBTree);

// returns true if the tree is empty, false otherwise
bool is_rbt_empty(const RBTree);

// changes the destroy function and returns the old one
DestroyFunc rbt_set_destroy(const RBTree, const DestroyFunc);

// destroys the memory used by the tree
void rbt_destroy(const RBTree);

//////////////////////////////
// tree traversal functions //
//////////////////////////////
typedef struct tnode* RBTreeNode;  // node handle

// returns the value of the node
Pointer rbt_node_value(const RBTreeNode);

// returns the node with that value, if it exists, otherwise NULL
RBTreeNode rbt_find_node(const RBTree, const Pointer);

// returns the previous, in order, node of target or NULL if there is no previous value
RBTreeNode rbt_find_previous(const RBTreeNode);

// returns the next, in order, node of target or NULL if there is no next value
RBTreeNode rbt_find_next(const RBTreeNode);

// returns the node with the lowest value
RBTreeNode rbt_first(const RBTree);

// returns the node with the highest value
RBTreeNode rbt_last(const RBTree);
