#include <time.h>
#include "../lib/ADT.h"
#include "./include/common.h"

#define NUM_OF_ELEMENTS 100000

void test_create(void)
{
    PQueue pq = pq_create(compareFunction, free);
    TEST_ASSERT(pq != NULL);
    TEST_ASSERT(pq_size(pq) == 0 && is_pq_empty(pq));
    pq_destroy(pq);
}

void test_insert(void)
{
    // create hash table
    PQueue pq = pq_create(compareFunction, free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_random_array(NUM_OF_ELEMENTS);

    clock_t cur_time = clock();        
    
    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        // insert the value
        pq_insert(pq, createData(arr[i]));

        // the size has changed
        TEST_ASSERT(pq_size(pq) == i+1);
    } 

    double time_insert = calc_time(cur_time);  // calculate insert time

    // free memory used
    pq_destroy(pq);
    free(arr);

    // report time taken
    printf("\n\nInsertion took %f seconds to complete\n", time_insert);
}

void test_remove(void)
{
    // create hash table
    PQueue pq = pq_create(compareFunction, free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_ordered_array(NUM_OF_ELEMENTS);

    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
        pq_insert(pq, createData(arr[i]));

    clock_t cur_time = clock();
    
    for (uint32_t i = NUM_OF_ELEMENTS-1; i > 0; i--)
    {
        TEST_ASSERT( *((int*)pq_remove(pq)) == arr[i]);

        // the size has changed
        TEST_ASSERT(pq_size(pq) == i);
    } 

    double time_insert = calc_time(cur_time);  // calculate remove time

    // free memory used
    pq_destroy(pq);
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
