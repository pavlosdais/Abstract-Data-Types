#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "../lib/ADT.h"

// function prototypes
int compareFunction(Pointer v1, Pointer v2);
int* createData(int a);

#define NUM_OF_ELEMENTS 100000

int main(void)
{
    // create rbt table
    RBTree rbt;
    rbt_init(&rbt, compareFunction, free);

    // create random input
    int* arr = malloc(sizeof(int) * NUM_OF_ELEMENTS);
    assert(arr != NULL);  // allocation failure

    unsigned int i;
    time_t t;
    srand((unsigned) time(&t));
    for (i = 0; i < NUM_OF_ELEMENTS; i++)
        arr[i] = rand() % RAND_MAX;

    
    clock_t cur_time;
    double time_insert, time_search, time_delete;

    // test insert
    cur_time = clock();
    for (i = 0; i < NUM_OF_ELEMENTS; i++)
        rbt_insert(rbt, createData(arr[i]));
    time_insert = ((double)(clock() - cur_time))/CLOCKS_PER_SEC;  // calculate insert time

    // test search
    unsigned int out_of_place = 0;
    cur_time = clock();
    for (i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        if (!rbt_exists(rbt, arr+i))
            out_of_place++;
    }
    time_search = ((double)(clock() - cur_time))/CLOCKS_PER_SEC;  // calculate search time
    
    i = rbt_size(rbt);
    printf("Total number of elements after insertion: %d\n", i);
    if (out_of_place != 0)
        printf("ERROR IN INSERTING");
    else
        printf("NO ERROR IN INSERTING");
    printf("\n\n");
    
    // test remove
    uint a = 0, to_be_deleted = (rand() % (i/4)) + (rand() % (i/2));
    cur_time = clock();
    for (uint j = 0; j < to_be_deleted; j++)
        a += rbt_remove(rbt, arr+j);
    time_delete = ((double)(clock() - cur_time))/CLOCKS_PER_SEC;  // calculate remove time

    printf("Deleted %d items\n", a);
    printf("Total number of elements after deletion: %d\n", rbt_size(rbt));
    if (rbt_size(rbt) + a != i)
        printf("ERROR IN DELETION");
    else
        printf("NO ERROR IN DELETION");

    // free the red-black tree
    rbt_destroy(rbt);
    free(arr);

    // report benchmarks
    printf("\n\nInsertion took %f seconds to execute\n", time_insert);
    printf("Search took %f seconds to execute\n", time_search);
    printf("Deletion took %f seconds to execute\n", time_delete);
    
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
