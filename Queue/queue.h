#include <stdbool.h>

typedef void* Pointer;

// Pointer to function that destroys an element value
typedef void (*DestroyFunc)(Pointer value);

typedef struct queue* Queue;

// initialize queue
void queue_init(Queue* Q, DestroyFunc destroy);

// return the size of the queue
unsigned int queue_size(Queue Q);

// enqueue value at the end of the queue
void queue_enqueue(Queue Q, Pointer value);

// dequeue value from the start of the queue
Pointer queue_dequeue(Queue Q);

// change the destroy function and return the old one
DestroyFunc queue_set_destroy_value(Queue Q, DestroyFunc new_destroy_func);

// free queue and its values if a destroy function is given
void queue_destroy(Queue Q);
