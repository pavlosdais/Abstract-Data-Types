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

typedef struct Set* RBTree;


// initializes red-black tree
void rbt_init(RBTree*, CompareFunc, DestroyFunc);

// returns true if the item is inserted, in any other case false
bool rbt_insert(RBTree, Pointer value);

// returns true if the item is deleted, in any other case false
bool rbt_remove(RBTree, Pointer value);

// returns true if the value exists, false otherwise
bool rbt_exists(RBTree, Pointer value);

// returns the number of elements in the tree
uint rbt_size(RBTree);

// returns true if the tree is empty, false otherwise
bool is_rbt_empty(RBTree);

// changes the destroy function and returns the old one
DestroyFunc rbt_set_destroy(RBTree, DestroyFunc);

// destroys the tree and its values if a destroy function was given
void rbt_destroy(RBTree);

//////////////////////////////
// tree traversal functions //
//////////////////////////////
typedef struct tnode* RBTreeNode;  // node handle

// returns the value of the node
Pointer rbt_node_value(RBTreeNode);

// returns the node with that value, if it exists, otherwise NULL
RBTreeNode rbt_find_node(RBTree, Pointer value);

// returns the previous, in order, node of target or NULL if there is no previous value
RBTreeNode rbt_find_previous(RBTreeNode);

// returns the next, in order, node of target or NULL if there is no next value
RBTreeNode rbt_find_next(RBTreeNode);

// returns the node with the lowest value
RBTreeNode rbt_first(RBTree);

// returns the node with the highest value
RBTreeNode rbt_last(RBTree);
