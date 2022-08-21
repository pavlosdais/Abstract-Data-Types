#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../lib/ADT.h"

// Function Prototypes
int compareFunction(Pointer v1, Pointer v2);
int* createData(int a);
void printFunc(Pointer value);

int main(void)
{
    // create priority queue
    PQueue pq;
    pq_init(&pq, compareFunction, free);

    // test insert
    int a = 1000;
    pq_insert(pq, createData(a));
    a = 8000;
    pq_insert(pq, createData(a));
    a = 5000;
    pq_insert(pq, createData(a));
    a = 10000;
    pq_insert(pq, createData(a));
    a = 7000;
    pq_insert(pq, createData(a));

    printf("Total number of elements after insertion: %d\n", pq_size(pq));

    // test remove
    int *b = pq_remove(pq);
    printFunc(b); printf("\n");
    free(b);

    b = pq_remove(pq);
    printFunc(b); printf("\n");
    free(b);

    b = pq_remove(pq);
    printFunc(b); printf("\n");
    free(b);

    printf("Total number of elements after deletion: %d\n", pq_size(pq));

    // free priority queue
    pq_destroy(pq);

    return 0;
}

int* createData(int a)
{
    int* val = malloc(sizeof(int));
    assert(val != NULL);  // allocation failure

    *val = a;
    return val;
}

// compare function
int compareFunction(Pointer v1, Pointer v2)
{
    return *((int*)v1) - *((int*)v2);
}

// print function
void printFunc(Pointer value)
{
    int* val = (int*)(value);
    printf("%d ", *val);
}
