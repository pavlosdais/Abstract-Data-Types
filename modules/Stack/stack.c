#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

typedef struct StackNode
{
    Pointer value;
    struct StackNode* next;
}
StackNode;
typedef struct StackNode* StackNodePointer;

typedef struct StackSet
{
    StackNodePointer top;  // node at the top of the stack, NULL if the stack is empty
    uint size;             // number of elements in the stack
    DestroyFunc destroy;   // function that destroys the elements, NULL if not
}
StackSet;

void stack_init(Stack* S, DestroyFunc destroy)
{
    *S = malloc(sizeof(StackSet));
    assert(*S != NULL);  // allocation failure
    
    (*S)->top = NULL;
    (*S)->destroy = destroy;
    (*S)->size = 0;
}

uint stack_size(Stack S)
{
    assert(S != NULL);
    return S->size;
}

bool is_stack_empty(Stack S)
{
    assert(S != NULL);
    return (S->top == NULL);
}

Pointer stack_top_value(Stack S)
{
    assert(S != NULL);
    return (S->top->value);
}

DestroyFunc stack_set_destroy(Stack S, DestroyFunc new_destroy_func)
{
    assert(S != NULL);
    
    DestroyFunc old_destroy_func = S->destroy;
    S->destroy = new_destroy_func;
    return old_destroy_func;
}

void stack_push(Stack S, Pointer value)
{
    assert(S != NULL);

    StackNodePointer* head = &(S->top);

    // create a new node
    StackNodePointer new_node = malloc(sizeof(StackNode));
    assert(new_node != NULL);  // allocation failure

    // fill the node's contents
    new_node->value = value;

    new_node->next = *head;
    *head = new_node;

    S->size++;  // value pushed, increment the number of elements in the stack
}

Pointer stack_pop(Stack S)
{
    if (is_stack_empty(S))
        return NULL;

    StackNodePointer head = S->top;

    Pointer data = head->value;
    StackNodePointer tmp = head;

    S->top = S->top->next;

    free(tmp);

    S->size--;  // value popped, decrement the number of elements in the stack
    return data;
}

void stack_destroy(Stack S)
{
    assert(S != NULL);
    
    StackNodePointer head = S->top;

    while(head != NULL)
    {
        StackNodePointer tmp = head;

        head = head->next;

        if (S->destroy != NULL)
            S->destroy(tmp->value);
        free(tmp);
    }

    free(S);
}
