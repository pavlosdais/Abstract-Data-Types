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
    DestroyFunc destroy    // function that destroys the elements, NULL if not
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
    return Q->num_of_elements;
}

const bool is_queue_empty(Queue Q)
{
    return (Q->head == NULL);
}

void queue_enqueue(Queue Q, Pointer value)
{
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

Pointer queue_dequeue(Queue Q)
{
    assert(!is_queue_empty(Q));  // empty queue, can not dequeue
    
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

DestroyFunc queue_set_destroy_value(Queue Q, DestroyFunc new_destroy_func)
{
    DestroyFunc old_destroy_func = Q->destroy;
    Q->destroy = new_destroy_func;
    return old_destroy_func;
}

void queue_destroy(Queue Q)
{
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
