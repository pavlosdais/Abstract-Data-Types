#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "../lib/ADT.h"

// Function Prototypes
int compareFunction(Pointer v1, Pointer v2);
int* createData(int a);

#define NUM_OF_ELEMENTS 1000

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

    // test insert
    for (i = 0; i < NUM_OF_ELEMENTS; i++)
        hash_insert(ht, createData(arr[i]));

    // test search
    unsigned int out_of_place = 0;
    for (i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        if (!hash_exists(ht, arr+i))
            out_of_place++;
    }
    
    i = hash_size(ht);
    printf("Total number of elements after insertion: %d\n", i);
    if (out_of_place != 0)
        printf("ERROR IN INSERTING");
    else
        printf("NO ERROR IN INSERTING");
    printf("\n\n");
    
    // test remove
    uint a = 0, to_be_deleted = (rand() % (i/4)) + (rand() % (i/2));
    for (uint j = 0; j < to_be_deleted; j++)
        a += hash_remove(ht, arr+j);

    printf("Deleted %d items\n", a);
    printf("Total number of elements after deletion: %d\n", hash_size(ht));
    if (hash_size(ht) + a != i)
        printf("ERROR IN DELETION\n");
    else
        printf("NO ERROR IN DELETION\n");

    // free hash_table
    hash_destroy(ht);
    free(arr);

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
