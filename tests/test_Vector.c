#include <time.h>
#include "../lib/ADT.h"
#include "./include/common.h"

#define NUM_OF_ELEMENTS 20000

void test_create(void)
{
    Vector vec = vector_create(free);
    TEST_ASSERT(vec != NULL);
    TEST_ASSERT(vector_size(vec) == 0 && is_vector_empty(vec));
    vector_destroy(vec);
}

void test_push_back(void)
{
    // create vector
    Vector vec = vector_create(free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_random_array(NUM_OF_ELEMENTS);
    
    for (uint64_t i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        // insert the value
        vector_push_back(vec, createData(arr[i]));

        TEST_ASSERT(*((int*)vector_at(vec, i)) == arr[i]);

        // the size has changed
        TEST_ASSERT(vector_size(vec) == i+1);
    } 

    // free memory used
    vector_destroy(vec);
    free(arr);
}

void test_clear_at(void)
{
    // create vector
    Vector vec = vector_create(free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_random_array(NUM_OF_ELEMENTS);

    for (uint64_t i = 0; i < NUM_OF_ELEMENTS; i++)
        vector_push_back(vec, createData(arr[i]));
    
    for (uint64_t i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        // insert the value
        vector_clear_at(vec, i);

        TEST_ASSERT(vector_at(vec, i) == NULL);

        // the size has changed
        TEST_ASSERT(vector_size(vec) == NUM_OF_ELEMENTS-1-i);
    } 

    // free memory used
    vector_destroy(vec);
    free(arr);
}

void test_search(void)
{
    // create vector
    Vector vec = vector_create(free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_random_array(NUM_OF_ELEMENTS);

    for (uint64_t i = 0; i < NUM_OF_ELEMENTS; i++)
        vector_push_back(vec, createData(arr[i]));
    
    clock_t cur_time = clock();

    for (uint64_t i = 0; i < NUM_OF_ELEMENTS; i++)
        TEST_ASSERT(vector_search(vec, arr+i, compareFunction));

    double time_insert = calc_time(cur_time);  // calculate search time

    // free memory used
    vector_destroy(vec);
    free(arr);

    printf("\n\nSearch took %f seconds to complete\n", time_insert);
}

void test_sort(void)
{
    // create vector
    Vector vec = vector_create(free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_shuffled_array(NUM_OF_ELEMENTS);

    for (uint64_t i = 0; i < NUM_OF_ELEMENTS; i++)
        vector_push_back(vec, createData(arr[i]));
    
    vector_sort(vec, compareFunction);

    for (int i = 0; i < NUM_OF_ELEMENTS; i++)
        TEST_ASSERT(*((int*)vector_at(vec, i)) == i);

    // free memory used
    vector_destroy(vec);
    free(arr);
}

void test_binary_search(void)
{
    // create vector
    Vector vec = vector_create(free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_random_array(NUM_OF_ELEMENTS);

    for (uint64_t i = 0; i < NUM_OF_ELEMENTS; i++)
        vector_push_back(vec, createData(arr[i]));

    clock_t cur_time = clock();

    vector_sort(vec, compareFunction);
    
    for (uint64_t i = 0; i < NUM_OF_ELEMENTS; i++)
        TEST_ASSERT(vector_binary_search(vec, arr+i, compareFunction));

    double time_insert = calc_time(cur_time);  // calculate binary search time

    // free memory used
    vector_destroy(vec);
    free(arr);

    printf("\n\nBinary search took %f seconds to complete\n", time_insert);
}

TEST_LIST = {
        { "create", test_create  },
        { "push back", test_push_back  },
        { "clear at", test_clear_at  },
        { "search", test_search  },
        { "sort", test_sort  },
        { "binary search", test_binary_search  },
        { NULL, NULL }
};
