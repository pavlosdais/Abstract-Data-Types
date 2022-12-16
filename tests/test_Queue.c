#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../lib/ADT.h"

// function prototypes
int* createData(int a);
void printFunc(Pointer value);
int compareFunction(Pointer v1, Pointer v2);

int main(void)
{
    // create queue
    Queue Q;
    queue_init(&Q, free);

    // test enqueue
    int a = 1000;
    queue_enqueue(Q, createData(a));
    a = 8000;
    queue_enqueue(Q, createData(a));
    a = 5000;
    queue_enqueue(Q, createData(a));
    a = 10000;
    queue_enqueue(Q, createData(a));
    a = 7000;
    queue_enqueue(Q, createData(a));

    printf("Total number of elements after insertion: %d\n", queue_size(Q));

    /*
    Queue, by now, looks like this:
    7000  <-- rear
    10000
    5000
    8000
    1000  <-- front
    */

    // test dequeue
    int *b = queue_dequeue(Q);
    printFunc(b); printf("\n");
    free(b);

    b = queue_dequeue(Q);
    printFunc(b); printf("\n");
    free(b);

    b = queue_dequeue(Q);
    printFunc(b); printf("\n");
    free(b);

    printf("Total number of elements after deletion: %d\n", queue_size(Q));

    // free queue
    queue_destroy(Q);

    return 0;
}

int* createData(int a)
{
    int* val = malloc(sizeof(int));
    assert(val != NULL);  // allocation failure

    *val = a;
    return val;
}

// print function
void printFunc(Pointer value)
{
    int* val = (int*)(value);
    printf("%d ", *val);
}

// compare function (for sorted insert)
int compareFunction(Pointer v1, Pointer v2)
{
    return *((int*)v1) - *((int*)v2);
}