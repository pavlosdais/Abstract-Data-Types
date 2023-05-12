#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pq.h"

// heap's minimum starting size
#define MIN_SIZE 64

#define ROOT 1
#define find_parent(a) (a/2)
#define find_left_child(a) (a*2)
#define find_right_child(a) (a*2 + 1)

typedef struct node
{
    Pointer data;
}
node;

typedef struct pq
{
    node* arr;            // array of nodes containing the data
    uint64_t curr_size;   // current size of the heap
    uint64_t capacity;    // max capacity of the heap
    CompareFunc compare;  // function that compares the data - dictates the order of the elements
    DestroyFunc destroy;  // function that destroys the elements, NULL if not
}
pq;

// function prototypes
static inline void swap_nodes(node*, node*);
static inline void bubble_up(const PQueue, uint64_t);
static void bubble_down(const PQueue, const uint64_t);

PQueue pq_create(const CompareFunc compare, const DestroyFunc destroy)
{
    assert(compare != NULL);

    PQueue PQ = malloc(sizeof(pq));
    assert(PQ != NULL);  // allocation failure
    
    // allocate memory for the array of nodes
    PQ->arr = calloc(MIN_SIZE, sizeof( *(PQ->arr)) );

    assert(PQ->arr != NULL);  // allocation failure

    PQ->curr_size = 0;
    PQ->capacity = MIN_SIZE;
    PQ->compare = compare;
    PQ->destroy = destroy;

    return PQ;
}

uint64_t pq_size(const PQueue PQ)
{
    assert(PQ != NULL);
    return PQ->curr_size;
}

bool is_pq_empty(const PQueue PQ)
{
    assert(PQ != NULL);
    return PQ->curr_size == 0;
}

Pointer pq_peek(const PQueue PQ)  { return PQ->arr[ROOT].data; }

void pq_insert(const PQueue PQ, const Pointer value)
{
    assert(PQ != NULL);

    PQ->curr_size++;

    // heap is full, double its size
    if (PQ->curr_size == PQ->capacity)
    {
        PQ->capacity *= 2;

        PQ->arr = realloc(PQ->arr, PQ->capacity * sizeof(*(PQ->arr)));
        assert(PQ->arr != NULL);  // allocation failure
    }

    PQ->arr[PQ->curr_size].data = value;

    bubble_up(PQ, PQ->curr_size);
}

static inline void bubble_up(const PQueue PQ, uint64_t node)
{
    uint64_t parent = find_parent(node);

    // bubble up until you find a tree node
    while (node != ROOT && PQ->compare(PQ->arr[parent].data, PQ->arr[node].data) < 0)
    {
        swap_nodes(&(PQ->arr[parent]), &(PQ->arr[node]));

        node = parent;
        parent = find_parent(node);
    }
}

Pointer pq_remove(const PQueue PQ)
{
    if (is_pq_empty(PQ))  // empty priority queue - nothing to remove
        return NULL;
    
    // save the element with the highest priority (which is at the root) and mark it as removed by making it NULL
    const Pointer hp = PQ->arr[ROOT].data;
    PQ->arr[ROOT].data = NULL;
    
    // root and far right leaf swap
    swap_nodes(&(PQ->arr[ROOT]), &(PQ->arr[PQ->curr_size]));

    PQ->curr_size--;

    bubble_down(PQ, ROOT);
    
    return hp;
}

static void bubble_down(const PQueue PQ, const uint64_t node)
{
    uint64_t left_child = find_left_child(node);
    if (left_child > PQ->curr_size)  // children do not exist
        return;
    
    // find the child with the highest priority
    uint64_t right_child = find_right_child(node), max_child = left_child;
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

DestroyFunc pq_set_destroy(const PQueue PQ, const DestroyFunc new_destroy_func)
{
    assert(PQ != NULL);

    DestroyFunc old_destroy_func = PQ->destroy;
    PQ->destroy = new_destroy_func;
    return old_destroy_func;
}

void pq_destroy(const PQueue PQ)
{
    assert(PQ != NULL);
    
    // if a destroy function was given, destroy the data
    if (PQ->destroy != NULL)
    {
        for (uint64_t i = 0, size = PQ->curr_size+1; i < size; i++)
            PQ->destroy(PQ->arr[i].data);
    }
    free(PQ->arr);
    free(PQ);
}

static inline void swap_nodes(node* a, node* b)
{
    node* tmp = a->data;
    a->data = b->data;
    b->data = tmp;
}
