#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

typedef struct QueueNode
{
    Pointer value;
    struct QueueNode* next;
}
QueueNode;

typedef struct queue
{
    QueueNode* head;       // head node
    QueueNode* tail;       // tail node
    uint num_of_elements;  // number of elements in the queue
    DestroyFunc destroy;   // function that destroys the elements, NULL if not
}
queue;

void queue_init(Queue* Q, DestroyFunc destroy)
{
    *Q = malloc(sizeof(queue));
    assert(*Q != NULL);  // allocation failure

    (*Q)->head = NULL;
    (*Q)->tail = NULL;
    (*Q)->destroy= destroy;
    (*Q)->num_of_elements = 0;
}

uint queue_size(Queue Q)
{
    assert(Q != NULL);
    return Q->num_of_elements;
}

bool is_queue_empty(Queue Q)
{
    assert(Q != NULL);
    return (Q->head == NULL);
}

void queue_enqueue(Queue Q, Pointer value)
{
    assert(Q != NULL);

    // allocate memory for the new node
    QueueNode* new_node = malloc(sizeof(QueueNode));
    assert(new_node != NULL);  // allocation failure

    // fill the node
    new_node->value = value;
    new_node->next = NULL;

    if (Q->tail != NULL)  // queue was not empty previously
        Q->tail->next = new_node;

    Q->tail = new_node;

    // queue was previously empty, reset
    if (is_queue_empty(Q))
        Q->head = new_node;

    // pushing at the end was successful, increase the number of elements by 1
    Q->num_of_elements++;
}

void queue_sorted_insert(Queue Q, Pointer value, CompareFunc compare)
{
    assert(Q != NULL);

    QueueNode** Queue = &(Q->head);  // pointer to the head of the node

    // create a new node
    QueueNode* new_node = malloc(sizeof(QueueNode));
    assert(new_node != NULL);  // allocation failure

    new_node->value = value;

    // empty queue - the new node becomes the head of the queue
    if (is_queue_empty(Q))
    {
        new_node->next = NULL;
        (*Queue) = new_node;
        Q->num_of_elements = 1;
        return;
    }
    
    Q->num_of_elements++;

    // the head of the node has less priority than the new node, meaning the new value should be first
    if (compare(value, (*Queue)->value) < 0)
    {
        new_node->next = (*Queue);
        (*Queue) = new_node;
        return;
    }

    // in any other case, traverse the list and find the correct position to insert the new node
    QueueNode* tmp = (*Queue);
    while (tmp->next != NULL && compare(tmp->next->value, value) < 0) tmp = tmp->next;

    // put the node at its place
    new_node->next = tmp->next;
    tmp->next = new_node;
}

Pointer queue_dequeue(Queue Q)
{
    if (is_queue_empty(Q))
        return NULL;
    
    QueueNode* tmp = Q->head;
    Pointer value = tmp->value;

    Q->head = Q->head->next;
    free(tmp);

    // queue is now empty, reset
    if (is_queue_empty(Q))
        Q->tail = NULL;

    // decrease the number of elements by 1 and return the dequeued element
    Q->num_of_elements--;
    return value;
}

DestroyFunc queue_set_destroy(Queue Q, DestroyFunc new_destroy_func)
{
    assert(Q != NULL);

    DestroyFunc old_destroy_func = Q->destroy;
    Q->destroy = new_destroy_func;
    return old_destroy_func;
}

void queue_destroy(Queue Q)
{
    assert(Q != NULL);

    while (Q->head != NULL)
    {
        // destroy the value of the node if a destroy function is given
        if (Q->destroy != NULL)
            Q->destroy(Q->head->value);
        
        QueueNode* tmp = Q->head->next;
        free(Q->head);
        Q->head = tmp;
    }
    free(Q);
}