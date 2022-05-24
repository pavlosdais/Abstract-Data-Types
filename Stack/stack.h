typedef void* Pointer;

// Pointer to function that destroys an element value
typedef void (*DestroyFunc)(Pointer value);

typedef struct StackSet* Stack;

// initialize stack
void stack_init(Stack* S, DestroyFunc destroy);

// return number of elements in the stack
unsigned int stack_size(Stack S);

// push value at the top of the stack
void stack_push(Stack S, Pointer value);

// pop value from the top of the stack
Pointer stack_pop(Stack S);

// return the value at the top of the stack
Pointer stack_top_value(Stack S);

// change the destroy function and return the old one
DestroyFunc stack_set_destroy_value(Stack S, DestroyFunc new_destroy_func);

// free stack
void stack_destroy(Stack S);
