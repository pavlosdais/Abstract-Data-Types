#pragma once  // include at most once

#include <stdbool.h>

typedef void* Pointer;

// Pointer to function that destroys an element value
typedef void (*DestroyFunc)(Pointer value);

typedef struct StackSet* Stack;


// initializes stack
void stack_init(Stack*, DestroyFunc destroy);

// pushes value at the top of the stack
void stack_push(Stack, Pointer value);

// pops value from the top of the stack and returns it
Pointer stack_pop(Stack);

// returns the size of the stack
uint stack_size(Stack);

// returns true if the stack is empty, false otherwise
bool is_stack_empty(Stack);

// returns the value at the top of the stack
Pointer stack_top_value(Stack);

// changes the destroy function and returns the old one
DestroyFunc stack_set_destroy(Stack, DestroyFunc);

// frees stack and its values if a destroy function is given
void stack_destroy(Stack);
