#pragma once  // include at most once

typedef void* Pointer;

// Pointer to function that destroys an element value
typedef void (*DestroyFunc)(Pointer value);

typedef struct StackSet* Stack;


// initializes stack
void stack_init(Stack* S, DestroyFunc destroy);

// pushes value at the top of the stack
void stack_push(Stack S, Pointer value);

// pops value from the top of the stack and returns it
Pointer stack_pop(Stack S);

// returns the size of the stack
uint stack_size(Stack S);

// returns the value at the top of the stack
Pointer stack_top_value(Stack S);

// changes the destroy function and returns the old one
DestroyFunc stack_set_destroy(Stack S, DestroyFunc new_destroy_func);

// frees stack and its values if a destroy function is given
void stack_destroy(Stack S);
