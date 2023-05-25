#include <time.h>
#include "../lib/ADT.h"
#include "./include/common.h"

#define NUM_OF_ELEMENTS 2000000

void test_create(void)
{
    HashTable ht = hash_create(hash_int1, compareFunction, free);
    TEST_ASSERT(ht != NULL);
    TEST_ASSERT(hash_size(ht) == 0 && is_ht_empty(ht));
    hash_destroy(ht);
}

void test_insert(void)
{
    // create hash table
    HashTable ht = hash_create(hash_int1, compareFunction, free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_shuffled_array(NUM_OF_ELEMENTS);

    clock_t cur_time = clock();
    
    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        // the value does not exist
        TEST_ASSERT(!hash_exists(ht, arr+i));

        // insert the value
        hash_insert(ht, createData(arr[i]));

        // the value now exists
        TEST_ASSERT(hash_exists(ht, arr+i));

        // the size has changed
        TEST_ASSERT(hash_size(ht) == i+1);
    }

    double time_insert = calc_time(cur_time);  // calculate insert time

    // free memory used
    hash_destroy(ht);
    free(arr);

    // report time taken
    printf("\n\nInsertion took %f seconds to complete\n", time_insert);
}

void test_remove(void)
{
    // create hash table
    HashTable ht = hash_create(hash_int1, compareFunction, free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_shuffled_array(NUM_OF_ELEMENTS);

    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
        hash_insert(ht, createData(arr[i]));

    clock_t cur_time = clock();
    
    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        // the value exists
        TEST_ASSERT(hash_exists(ht, arr+i));

        // remove the value
        hash_remove(ht, arr+i);

        // the value now does not exist
        TEST_ASSERT(!hash_exists(ht, arr+i));

        // the size has changed
        TEST_ASSERT(hash_size(ht) == NUM_OF_ELEMENTS-i-1);
    } 

    double time_insert = calc_time(cur_time);  // calculate remove time

    // free memory used
    hash_destroy(ht);
    free(arr);

    // report time taken
    printf("\n\nRemove took %f seconds to complete\n", time_insert);
}

TEST_LIST = {
        { "create", test_create  },
        { "insert", test_insert  },
        { "remove", test_remove  },
        { NULL, NULL }
};
