#pragma once  // include at most once

#include <stdbool.h>
#include <stdint.h>

typedef void* Pointer;

// Pointer to function that destroys an element value
typedef void (*DestroyFunc)(Pointer value);

typedef struct StackSet* Stack;


// creates stack
Stack stack_create(const DestroyFunc);

// pushes value at the top of the stack
void stack_push(const Stack, const Pointer value);

// pops value from the top of the stack and returns it, returns NULL if the stack is empty
Pointer stack_pop(const Stack);

// returns the size of the stack
uint64_t stack_size(const Stack);

// returns true if the stack is empty, false otherwise
bool is_stack_empty(const Stack);

// returns the value at the top of the stack
Pointer stack_top_value(const Stack);

// changes the destroy function and returns the old one
DestroyFunc stack_set_destroy(const Stack, const DestroyFunc);

// frees stack and its values if a destroy function is given
void stack_destroy(const Stack);
