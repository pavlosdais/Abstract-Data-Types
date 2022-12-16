#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../lib/ADT.h"

// function prototypes
int* createData(int a);
void printFunc(Pointer value);

int main(void)
{
    // create stack
    Stack st;
    stack_init(&st, free);

    // test push
    int a = 1000;
    stack_push(st, createData(a));
    a = 8000;
    stack_push(st, createData(a));
    a = 5000;
    stack_push(st, createData(a));
    a = 10000;
    stack_push(st, createData(a));
    a = 7000;
    stack_push(st, createData(a));

    printf("Total number of elements after insertion: %d\n", stack_size(st));

    /*
    Stack, by now, looks like this:
    7000  <-- top
    10000
    5000
    8000
    1000
    */

    // test pop
    int *b = stack_pop(st);
    printFunc(b); printf("\n");
    free(b);

    b = stack_pop(st);
    printFunc(b); printf("\n");
    free(b);

    b = stack_pop(st);
    printFunc(b); printf("\n");
    free(b);

    printf("Total number of elements after deletion: %d\n", stack_size(st));

    // free stack
    stack_destroy(st);

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
