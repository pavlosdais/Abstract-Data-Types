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
void rbt_init(RBTree* Tree, CompareFunc compare, DestroyFunc destroy);

// returns true if the item is inserted, in any other case false
bool rbt_insert(RBTree Tree, Pointer value);

// returns true if the item is deleted, in any other case false
bool rbt_remove(RBTree Tree, Pointer value);

// returns true if the value exists, false otherwise
bool rbt_find(RBTree Tree, Pointer value);

// returns the number of elements in the tree
unsigned int rbt_size(RBTree Tree);

// changes the destroy function and returns the old one
DestroyFunc rbt_set_destroy(RBTree Tree, DestroyFunc new_destroy_func);

// destroys the tree and its values if a destroy function was given
void rbt_destroy(RBTree Tree);

//////////////////////////////
// tree traversal functions //
//////////////////////////////
typedef struct tnode* RBTreeNode;  // node handle

// returns the value of the node
Pointer rbt_node_value(RBTreeNode rbt_node);

// returns the node with that value, if it exists, otherwise NULL
RBTreeNode rbt_find_node(RBTree Tree, Pointer value);

// returns the node with the lowest value
RBTreeNode rbt_first(RBTree Tree);

// returns the node with the highest value
RBTreeNode rbt_last(RBTree Tree);

// returns the previous node of target, or NULL if there is no previous value
RBTreeNode rbt_find_previous(RBTree Tree, RBTreeNode target);

// returns the next node of target, or NULL if there is no next value
RBTreeNode rbt_find_next(RBTree Tree, RBTreeNode target);
