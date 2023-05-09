#include <time.h>
#include "../lib/ADT.h"
#include "./include/common.h"

#define NUM_OF_ELEMENTS 100000

void test_create(void)
{
    Queue Q = queue_create(free);
    TEST_ASSERT(Q != NULL);
    TEST_ASSERT(queue_size(Q) == 0 && is_queue_empty(Q));
    queue_destroy(Q);
}

void test_enqueue(void)
{
    // create queue
    Queue Q = queue_create(free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_random_array(NUM_OF_ELEMENTS);

    clock_t cur_time = clock();
    
    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        // insert the value
        queue_enqueue(Q, createData(arr[i]));

        // the size has changed
        TEST_ASSERT(queue_size(Q) == i+1);
    }

    double time_insert = calc_time(cur_time);  // calculate insert time

    // free memory used
    queue_destroy(Q);
    free(arr);

    // report time taken
    printf("\n\nEnqueue took %f seconds to complete\n", time_insert);
}

void test_dequeue(void)
{
    // create queue
    Queue Q = queue_create(free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_random_array(NUM_OF_ELEMENTS);

    clock_t cur_time = clock();

    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
        queue_enqueue(Q, createData(arr[i]));
    
    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        // insert the value
        TEST_ASSERT( *((int*)queue_dequeue(Q)) == arr[i]);

        // the size has changed
        TEST_ASSERT(queue_size(Q) == NUM_OF_ELEMENTS-i-1);
    }

    double time_insert = calc_time(cur_time);  // calculate insert time

    // free memory used
    queue_destroy(Q);
    free(arr);

    // report time taken
    printf("\n\nDequeue took %f seconds to complete\n", time_insert);
}

TEST_LIST = {
        { "create", test_create  },
        { "enqueue", test_enqueue  },
        { "dequeue", test_dequeue  },
        { NULL, NULL }
};
