#include <time.h>
#include "../lib/ADT.h"
#include "./include/common.h"

#define NUM_OF_ELEMENTS 10000000

void test_create(void)
{
    Stack st = stack_create(free);
    TEST_ASSERT(st != NULL);
    TEST_ASSERT(stack_size(st) == 0 && is_stack_empty(st));
    stack_destroy(st);
}

void test_push(void)
{
    // create stack
    Stack st = stack_create(free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_random_array(NUM_OF_ELEMENTS);

    clock_t cur_time = clock();
    
    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        // insert the value
        stack_push(st, createData(arr[i]));

        // the size has changed
        TEST_ASSERT(stack_size(st) == i+1);
    } 

    double time_insert = calc_time(cur_time);  // calculate insert time

    // free memory used
    stack_destroy(st);
    free(arr);

    // report time taken
    printf("\n\nPush took %f seconds to complete\n", time_insert);
}

void test_pop(void)
{
    // create stack
    Stack st = stack_create(free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_random_array(NUM_OF_ELEMENTS);

    clock_t cur_time = clock();

    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
        stack_push(st, createData(arr[i]));
    
    for (uint32_t i = NUM_OF_ELEMENTS-1; i > 0; i--)
    {
        // insert the value
        TEST_ASSERT( *((int*)stack_pop(st)) == arr[i]);

        // the size has changed
        TEST_ASSERT(stack_size(st) == i);
    } 

    double time_insert = calc_time(cur_time);  // calculate insert time

    // free memory used
    stack_destroy(st);
    free(arr);

    // report time taken
    printf("\n\nPop took %f seconds to complete\n", time_insert);
}

TEST_LIST = {
        { "create", test_create  },
        { "push", test_push  },
        { "pop", test_pop  },
        { NULL, NULL }
};
