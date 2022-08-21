#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pq.h"

#define ROOT 1

typedef struct n
{
    Pointer data;
}
n;
typedef struct n* node;

typedef struct pq
{
    node arr;             // array of nodes containing the data
    uint curr_size;       // current size of the heap
    uint capacity;        // max capacity of the heap
    CompareFunc compare;  // function that compares the data - dictates the order of the elements
    DestroyFunc destroy;  // function that destroys the elements, NULL if not
}
pq;

// Function prototypes
unsigned int pq_size(PQueue PQ);
static uint find_parent(uint node);
static uint find_left_child(uint node);
static uint find_right_child(uint node);
static void swap_nodes(node a, node b);
static void bubble_up(PQueue PQ, uint node);
static void bubble_down(PQueue PQ, uint node);

void pq_init(PQueue* PQ, CompareFunc compare, DestroyFunc destroy)
{
    assert(compare != NULL);

    *PQ = malloc(sizeof(pq));
    assert(*PQ != NULL);  // allocation failure
    
    // allocate memory for the array of nodes
    (*PQ)->arr = calloc(MIN_SIZE, sizeof( *((*PQ)->arr)) );

    assert((*PQ)->arr != NULL);  // allocation failure

    (*PQ)->capacity = MIN_SIZE;
    (*PQ)->compare = compare;
    (*PQ)->destroy = destroy;
    (*PQ)->curr_size = 0;
}

unsigned int pq_size(PQueue PQ)
{
    return PQ->curr_size;
}

void pq_insert(PQueue PQ, Pointer value)
{
    PQ->curr_size++;

    // heap is full, double its size
    if (PQ->curr_size == PQ->capacity)
    {
        node new_arr = realloc(PQ->arr, 2*PQ->capacity * sizeof(*new_arr));
        
        assert(new_arr != NULL);  // allocation failure
        
        PQ->capacity *= 2;
        PQ->arr = new_arr;
    }

    PQ->arr[PQ->curr_size].data = value;

    bubble_up(PQ, PQ->curr_size);
}

static void bubble_up(PQueue PQ, uint node)
{
    uint parent = find_parent(node);
    while (node != ROOT && PQ->compare(PQ->arr[parent].data, PQ->arr[node].data) < 0)
    {
        swap_nodes(&(PQ->arr[parent]), &(PQ->arr[node]));

        node = parent;
        parent = find_parent(node);
    }
}

Pointer pq_remove(PQueue PQ)
{
    if (pq_size(PQ) == 0)  // empty priority queue - nothing to remove
        return NULL;
    
    // save the element with the highest priority (which is at the root) and mark it as removed by making it NULL
    Pointer hp = PQ->arr[ROOT].data;
    PQ->arr[ROOT].data = NULL;
    
    // root and far right leaf swap
    swap_nodes(&(PQ->arr[ROOT]), &(PQ->arr[PQ->curr_size]));

    PQ->curr_size--;

    bubble_down(PQ, ROOT);
    
    return hp;
}

static void bubble_down(PQueue PQ, uint node)
{
    uint left_child = find_left_child(node);
    if (left_child > PQ->curr_size)  // children do not exist
        return;
    
    // find the child with the highest priority
    uint right_child = find_right_child(node);
    uint max_child = left_child;

    if (right_child <= PQ->curr_size && PQ->compare(PQ->arr[left_child].data, PQ->arr[right_child].data) < 0)
        max_child = right_child;
    
    // bubble down if the the child with the highest priority
    // has a higher priority than the current node
    if (PQ->compare(PQ->arr[node].data, PQ->arr[max_child].data) < 0)
    {
        swap_nodes(&(PQ->arr[node]), &(PQ->arr[max_child]));
        bubble_down(PQ, max_child);
    }
}

DestroyFunc pq_set_destroy(PQueue PQ, DestroyFunc new_destroy_func)
{
    DestroyFunc old_destroy_func = PQ->destroy;
    PQ->destroy = new_destroy_func;
    return old_destroy_func;
}

void pq_destroy(PQueue PQ)
{
    // if a destroy function was given
    if (PQ->destroy != NULL)
    {
        for (uint i = 0; i < PQ->capacity; i++)
        {
            // if the element has not already been removed before, destroy it
            if (PQ->arr[i].data != NULL)
                PQ->destroy(PQ->arr[i].data);
        }
    }
    free(PQ->arr);
    free(PQ);
}

static uint find_parent(uint node)
{
    return node / 2;
}

static uint find_left_child(uint node)
{
    return 2*node;
}

static uint find_right_child(uint node)
{
    return 2*node + 1;
}

static void swap_nodes(node a, node b)
{
    node tmp = a->data;
    a->data = b->data;
    b->data = tmp;
}
