#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "RedBlackTree.h"

// source: http://staff.ustc.edu.cn/~csli/graduate/algorithms/book6/chap14.htm

typedef enum COLORS
{
    RED,
    BLACK
}
COLORS;

typedef struct tnode
{
    Pointer data;
    COLORS col;  // represents the color of the node (Red/Black)

    struct tnode *left, *right, *parent;
}
tnode;
typedef struct tnode* RBTreeNode;

struct Set
{
    RBTreeNode root;      // root node, NULL if the the tree is empty
    uint size;            // number of elements in the tree
    CompareFunc compare;  // function that compares the elements - dictates the order of the elements
    DestroyFunc destroy;  // function that destroys the elements, NULL if not
};

// Function Prototypes
static void right_rotation(RBTreeNode* root, RBTreeNode* node);
static void left_rotation(RBTreeNode* root, RBTreeNode* node);
static RBTreeNode find_successor(RBTreeNode node);
static void shift_node(RBTreeNode* root, RBTreeNode a, RBTreeNode b);
static void fix_insert(RBTreeNode* root, RBTreeNode* node);
static void fix_remove(RBTreeNode* root, RBTreeNode* node);

tnode NULLNode = {NULL, BLACK, NULL, NULL, NULL};  // dummy leaf node

void rbt_init(RBTree* Tree, CompareFunc compare, DestroyFunc destroy)
{
    assert(compare != NULL);  // a compare function needs to be given

    *Tree = malloc(sizeof(struct Set));
    assert(*Tree != NULL);  // allocation failure
    
    (*Tree)->size = 0;
    (*Tree)->root = NULL;
    (*Tree)->compare = compare;
    (*Tree)->destroy = destroy;
}

uint rbt_size(RBTree Tree) { return Tree->size; }

// creates node
static RBTreeNode CreateNode()
{
    RBTreeNode new_node = malloc(sizeof(tnode));
    assert(new_node != NULL);  // allocation failure
    
    new_node->col = RED;  // default color is red
    new_node->left = &NULLNode;
    new_node->right = &NULLNode;
    return new_node;
}

// traverse the tree to find the node with the value
// returns NULL if the value is not found
RBTreeNode rbt_find_node(RBTree Tree, Pointer value)
{
    RBTreeNode node = Tree->root;
    
    while (node != &NULLNode)
    {
        int comp = Tree->compare(value, node->data);
        if (comp == 0)  // node->data == value
            return node;
        else if (comp < 0)  // value < node->data
            node = node->left;
        else  // value >= node->data
            node = node->right;
    }
    
    // value was not found
    return NULL;
}

Pointer rbt_node_value(RBTreeNode rbt_node) { return rbt_node->data; }

bool rbt_insert(RBTree Tree, Pointer value)
{
    RBTreeNode* root = &(Tree->root);

    RBTreeNode new_node = CreateNode();
    assert(new_node != NULL);  // allocation failure
    
    new_node->data = value;
    if (*root == NULL)  // empty tree
    {
        Tree->size = 1;
        new_node->col = BLACK;  // root is black
        new_node->parent = NULL;  // root's parent is NULL
        *root = new_node;
        return true;
    }
    
    // standard BST insertion - by the end the node prev will 
    // be the parent of the node we will insert
    RBTreeNode prev = NULL, tmp = *root;

    int prev_comp, comp = Tree->compare(tmp->data, value);
    while(true)
    {
        prev = tmp;
        prev_comp = comp;

        if (comp == 0)  // value already exists
        {
            free(new_node);

            // if a destroy function exists, destroy the value
            if (Tree->destroy != NULL)
                Tree->destroy(value);
            
            return false;
        }
        else if (comp < 0)  // tmp->data < value
            tmp = tmp->right;
        else  // tmp->data >= value
            tmp = tmp->left;
        
        if (tmp != &NULLNode)
            comp = Tree->compare(tmp->data, value);
        else break;
    }

    // save parent
    new_node->parent = prev;

    if (prev_comp < 0)
        prev->right = new_node;
    else
        prev->left = new_node;

    // fix possible violations
    fix_insert(root, &new_node);  

    Tree->size++;  // value inserted, increment the number of elements in the tree
    return true;
}

bool rbt_remove(RBTree Tree, Pointer value)
{
    RBTreeNode* root = &(Tree->root);
    
    RBTreeNode tmp = rbt_find_node(Tree, value);
    if (tmp == NULL)  // value does not exist
        return false;
    
    RBTreeNode node_to_be_deleted = tmp;

    COLORS col = tmp->col;   // save the color of the node that is about to be deleted
    if (node_to_be_deleted->left == &NULLNode)
    {
        tmp = node_to_be_deleted->right;
        shift_node(root, node_to_be_deleted, node_to_be_deleted->right);
    }
    else if (node_to_be_deleted->right == &NULLNode)
    {
        tmp = node_to_be_deleted->left;
        shift_node(root, node_to_be_deleted, tmp);
    }
    else  // node has 2 children
    {
        // Find the successor of the node, swap it with the node we want to delete
        // and delete the successor instead
        RBTreeNode successor = find_successor(node_to_be_deleted);
        col = successor->col;    // save the new color of the node
        tmp = successor->right;  // save the right child of the node we want to delete
        successor->col = node_to_be_deleted->col;  // keep the color same
        
        if (successor->parent == node_to_be_deleted)   // successor's parent is the node that we want to delete
            tmp->parent = successor;
        else
        {
            shift_node(root, successor, successor->right);
            successor->right = node_to_be_deleted->right;
            successor->right->parent = successor;
        }

        shift_node(root, node_to_be_deleted, successor);
        successor->left = node_to_be_deleted->left;
        successor->left->parent = successor;
        successor->col = node_to_be_deleted->col;
    }

    if (Tree->destroy != NULL)
        Tree->destroy(node_to_be_deleted->data);
    free(node_to_be_deleted);

    if (col == BLACK)            // no violations if the node deleted is red
        fix_remove(root, &tmp);  // if node is black, fix violations
    
    Tree->size--;  // value removed, decrement the number of elements in the tree
    return true;
}

// fix possible violations at insertion
static void fix_insert(RBTreeNode* root, RBTreeNode* node) 
{
    while(((*node) != *root) && ((*node)->col == RED) && ((*node)->parent->col == RED))
    {
        // store parent, grandparent and uncle
        RBTreeNode uncle = NULL, parent = (*node)->parent, grandparent = parent->parent;
   
        if (grandparent->left == parent)
            uncle = grandparent->right;
        else
            uncle = grandparent->left;
        
        if (uncle != NULL && uncle->col == RED)  // uncle is red, recolor as follows:
        {
            parent->col = uncle->col = BLACK;  // 1. Change the color of parent and uncle as black
            grandparent->col = RED;            // 2. Change the color of the grandparent as red
            (*node) = grandparent;             // 3. New node becomes its grandparent
        }
        else  // uncle is black, rotation
        {
            // There are 4 cases:
            // 1. parent is left child of grandparent and node is left child of parent (LL)
            // 2. parent is left child of grandparent and node is right child of parent (LR)
            // 3. parent is right child of grandparent and node is right child of parent (RR)
            // 4. parent is right child of grandparent and node is left child of parent (RL) 
            
            if (grandparent->left == parent)  // parent is left child of grandparent - Lx
            {
                if (parent->right == (*node))  // LR
                {
                    left_rotation(root, &parent);  // Left rotation of parent
                    (*node) = parent;
                    parent = (*node)->parent;
                }
                // Apply the steps of LL 
                right_rotation(root, &grandparent);  // 1. Right rotation of grandparent
                parent->col = BLACK;                 // 2. Color parent black
                grandparent->col = RED;              // and grandparent red
                (*node) = parent;  
            }
            else  // parent is right child of grandparent - Rx
            {
                if (parent->left == (*node))  // RL
                {
                    right_rotation(root, &parent);  // Right rotation of parent
                    (*node) = parent;
                    parent = (*node)->parent;
                }
                // Apply the steps of RR
                left_rotation(root, &grandparent);  // 1. Left rotation of grandparent
                parent->col = BLACK;                // 2. Color parent black
                grandparent->col = RED;             // and grandparent red
                (*node) = parent;
            }
        }
    }

    (*root)->col = BLACK;  // keep root black
}

// fix possible violations at removal
static void fix_remove(RBTreeNode* root, RBTreeNode* node)
{
    while ((*node) != *root && (*node)->col == BLACK)
    {
        // S = sibling, n = node
        if ((*node) == (*node)->parent->right)
        {
            RBTreeNode sibling = (*node)->parent->left;

            // -CASE 1:
            // S is red. Since s must have black children, we can switch the colors
            // of s and its parent and then perform a right-rotation on the parent
            // without violating any of the red-black properties. The new sibling of
            // node, one of s's children, is now black, and thus we have converted case
            // 1 into case 2, 3, or 4.
            if (sibling->col == RED)
            {
                sibling->col = BLACK;
                (*node)->parent->col = RED; 
                right_rotation(root, &((*node)->parent));
                sibling = (*node)->parent->left;
            }

            // -CASE 2:
            // S is black by now. If both of the children of s are black, since
            // s is black we make s red leaving only n with black color and s with
            // red. We then repeat the while loop with the parent as the node.
            if (sibling->right->col == BLACK && sibling->left->col == BLACK)
            {
                sibling->col = RED;
                (*node) = (*node)->parent;
            }
            else
            {
                // -CASE 3:
                // N is black, its right child is red and its left child is black.
                // We can switch the colors of the sibling and its right child and then
                // perform a left rotation on the sibling without violating any of the
                // red-black properties. The new sibling s of n is now a black node with
                // a red left child, and thus case 3 is transformed into case 4.
                if (sibling->left->col == BLACK)
                {
                    sibling->col = RED;
                    sibling->right->col = BLACK;
                    left_rotation(root, &sibling);
                    sibling = (*node)->parent->left;
                }

                // -CASE 4:
                // N's sibling is black and s's left child is red. By making some color
                // changes and performing a right rotation on its parent, we can remove the
                // extra black on node without violating any of the red-black properties.
                // We then terminate the loop by making the node the root.
                sibling->col = (*node)->parent->col;
                (*node)->parent->col = sibling->left->col = BLACK;
                right_rotation(root, &((*node)->parent));
                (*node) = (*root);  //  terminate
            }
        }
        else  // node == parent->left
        {
            // The cases here are mirror of the previous ones, if we swap left with right.
            RBTreeNode sibling = (*node)->parent->right;

            // CASE 1
            if (sibling->col == RED)
            {
                sibling->col = BLACK;
                (*node)->parent->col = RED;
                left_rotation(root, &((*node)->parent));
                sibling = (*node)->parent->right;
            }

            // CASE 2
            if (sibling->right->col == BLACK && sibling->left->col == BLACK)
            {
                sibling->col = RED;
                (*node) = (*node)->parent;
            }
            else
            {
                // CASE 3
                if (sibling->right->col == BLACK)
                {
                    sibling->col = RED;
                    sibling->left->col = BLACK;
                    right_rotation(root, &sibling);
                    sibling = (*node)->parent->right;
                }

                // CASE 4
                sibling->col = (*node)->parent->col;
                (*node)->parent->col = sibling->right->col = BLACK;
                left_rotation(root, &((*node)->parent));
                (*node) = (*root);  //  terminate
            }
        }
    }
    
    (*node)->col = BLACK;
}

// destroys the nodes of the tree and their data, if a destroy function is given
static void destroy_nodes(RBTreeNode node, DestroyFunc destroy_data)
{
    if (node == &NULLNode)  // base case
        return;

    // first destroy the children, then the data
    destroy_nodes(node->left, destroy_data);
    destroy_nodes(node->right, destroy_data);

    if (destroy_data != NULL)
        destroy_data(node->data);
    
    free(node);
}

void rbt_destroy(RBTree Tree)
{
    if (Tree->root != NULL)
        destroy_nodes(Tree->root, Tree->destroy);  // destroy the nodes
    free(Tree);                                    // then the tree
}

bool rbt_exists(RBTree Tree, Pointer value)
{
    return rbt_find_node(Tree, value) != NULL;
}

static RBTreeNode node_max(RBTreeNode node)
{
    RBTreeNode tmp = node;
    while (tmp->right != &NULLNode)
        tmp = tmp->right;

    return tmp;
}

static RBTreeNode node_min(RBTreeNode node)
{
    RBTreeNode tmp = node;
    while (tmp->left != &NULLNode)
        tmp = tmp->left;
    
    return tmp;
}

static RBTreeNode find_predecessor(RBTreeNode node)
{
    return node_max(node->left);
}

static RBTreeNode find_successor(RBTreeNode node)
{
    return node_min(node->right);
}

RBTreeNode rbt_find_previous(RBTreeNode target)
{
    if (target->left != &NULLNode)
        return find_predecessor(target);
    
    // left tree does not exist, the next in order node is one of the ancestors
    RBTreeNode parent = target->parent;
    while(parent != NULL && target == parent->left)
    {
        target = parent;
        parent = parent->parent;
    }

    return parent;
}

RBTreeNode rbt_find_next(RBTreeNode target)
{
    if (target->right != &NULLNode)
        return find_successor(target);
    
    // right tree does not exist, the previous in order node is one of the predecessors
    RBTreeNode parent = target->parent;
    while(parent != NULL && target == parent->right)
    {
        target = parent;
        parent = parent->parent;
    }

    return parent;
}

RBTreeNode rbt_first(RBTree Tree)
{
    return node_min(Tree->root);
}

RBTreeNode rbt_last(RBTree Tree)
{
    return node_max(Tree->root);
}

DestroyFunc rbt_set_destroy(RBTree Tree, DestroyFunc new_destroy_func)
{
    DestroyFunc old_destroy_func = Tree->destroy;
    Tree->destroy = new_destroy_func;
    return old_destroy_func;
}

// node b takes a's place
static void shift_node(RBTreeNode* root, RBTreeNode a, RBTreeNode b)
{
    if (a->parent == NULL)
        *root = b;
    else if (a == a->parent->right)
        a->parent->right = b;
    else
        a->parent->left = b;
    
    b->parent = a->parent;
}

// right rotation at node
static void right_rotation(RBTreeNode* root, RBTreeNode* node)
{
    RBTreeNode left_child = (*node)->left;
    (*node)->left = left_child->right;

    if (left_child->right != &NULLNode)
        left_child->right->parent = (*node);
    
    left_child->parent = (*node)->parent;

    if (*node == *root)
        (*root) = left_child;
    else if ((*node) == (*node)->parent->left)
        (*node)->parent->left = left_child;
    else
        (*node)->parent->right = left_child;
    
    left_child->right = (*node);
    (*node)->parent = left_child;
}

// left rotation at node
static void left_rotation(RBTreeNode* root, RBTreeNode* node)
{
    RBTreeNode right_child = (*node)->right;
    (*node)->right = right_child->left;

    if (right_child->left != &NULLNode)
        right_child->left->parent = (*node);

    right_child->parent = (*node)->parent;

    if ((*node) == *root)
        (*root) = right_child;
    else if ((*node) == (*node)->parent->left)
        (*node)->parent->left = right_child;
    else
        (*node)->parent->right = right_child;

    right_child->left = (*node);
    (*node)->parent = right_child;
}
