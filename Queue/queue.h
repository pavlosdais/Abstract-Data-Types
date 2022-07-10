#include <stdbool.h>

typedef void* Pointer;

// Pointer to function that destroys an element value
typedef void (*DestroyFunc)(Pointer value);

typedef struct queue* Queue;


// initializes queue
void queue_init(Queue* Q, DestroyFunc destroy);

// returns the size of the queue
uint queue_size(Queue Q);

// enqueues value at the end of the queue
void queue_enqueue(Queue Q, Pointer value);

// dequeues value from the start of the queue
Pointer queue_dequeue(Queue Q);

// changes the destroy function and return the old one
DestroyFunc queue_set_destroy_value(Queue Q, DestroyFunc new_destroy_func);

// frees queue and its values if a destroy function is given
void queue_destroy(Queue Q);
