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
    // create hash table
    HashTable ht;
    hash_init(&ht, hash_int, compareFunction, free);

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
        hash_insert(ht, createData(arr[i]));
    time_insert = ((double)(clock() - cur_time))/CLOCKS_PER_SEC;  // calculate insert time

    // test search
    unsigned int out_of_place = 0;
    cur_time = clock();
    for (i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        if (!hash_exists(ht, arr+i))
            out_of_place++;
    }
    time_search = ((double)(clock() - cur_time))/CLOCKS_PER_SEC;  // calculate search time
    
    i = hash_size(ht);
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
        a += hash_remove(ht, arr+j);
    time_delete = ((double)(clock() - cur_time))/CLOCKS_PER_SEC;  // calculate remove time

    printf("Deleted %d items\n", a);
    printf("Total number of elements after deletion: %d\n", hash_size(ht));
    if (hash_size(ht) + a != i)
        printf("ERROR IN DELETION");
    else
        printf("NO ERROR IN DELETION");

    // free the hash table
    hash_destroy(ht);
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
