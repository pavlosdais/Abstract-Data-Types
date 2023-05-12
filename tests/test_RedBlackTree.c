#include <time.h>
#include "../lib/ADT.h"
#include "./include/common.h"

#define NUM_OF_ELEMENTS 100000

void test_create(void)
{
    RBTree rbt = rbt_create(compareFunction, free);
    TEST_ASSERT(rbt != NULL);
    TEST_ASSERT(rbt_size(rbt) == 0 && is_rbt_empty(rbt));
    rbt_destroy(rbt);
}

void test_insert(void)
{
    // create create rbt
    RBTree rbt = rbt_create(compareFunction, free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_shuffled_array(NUM_OF_ELEMENTS);

    clock_t cur_time = clock();
    
    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        // the value does not exist
        TEST_ASSERT(!rbt_exists(rbt, arr+i));

        // insert the value
        rbt_insert(rbt, createData(arr[i]));

        // the value now exists
        TEST_ASSERT(rbt_exists(rbt, arr+i));

        // the size has changed
        TEST_ASSERT(rbt_size(rbt) == i+1);
    }

    double time_insert = calc_time(cur_time);  // calculate insert time

    // free memory used
    rbt_destroy(rbt);
    free(arr);

    // report time taken
    printf("\n\nInsertion took %f seconds to complete\n", time_insert);
}

void test_remove(void)
{
    // create create rbt
    RBTree rbt = rbt_create(compareFunction, free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_shuffled_array(NUM_OF_ELEMENTS);

    clock_t cur_time = clock();

    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
        rbt_insert(rbt, createData(arr[i]));
    
    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        // the value exists
        TEST_ASSERT(rbt_exists(rbt, arr+i));

        // remove the value
        rbt_remove(rbt, arr+i);

        // the value now does not exist
        TEST_ASSERT(!rbt_exists(rbt, arr+i));

        // the size has changed
        TEST_ASSERT(rbt_size(rbt) == NUM_OF_ELEMENTS-i-1);
    }

    double time_insert = calc_time(cur_time);  // calculate remove time

    // free memory used
    rbt_destroy(rbt);
    free(arr);

    // report time taken
    printf("\n\nDelete took %f seconds to complete\n", time_insert);
}

void test_traversal(void)
{
    // create rbt
    RBTree rbt = rbt_create(compareFunction, free);
    
    time_t t;
    srand((unsigned) time(&t));
    
    int* arr = create_shuffled_array(NUM_OF_ELEMENTS);

    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
        rbt_insert(rbt, createData(arr[i]));

    // test forward traversal
    int value = 0;
    for (RBTreeNode node = rbt_first(rbt); node != NULL; node = rbt_find_next(node), value++)
        TEST_ASSERT( *((int*)rbt_node_value(node)) == value);

    // test backward traversal
    value = NUM_OF_ELEMENTS-1;
    for (RBTreeNode node = rbt_last(rbt); node != NULL; node = rbt_find_previous(node), value--)
        TEST_ASSERT( *((int*)rbt_node_value(node)) == value);

    // free memory used
    rbt_destroy(rbt);
    free(arr);
}

TEST_LIST = {
        { "create", test_create  },
        { "insert", test_insert  },
        { "remove", test_remove  },
        { "traversal", test_traversal  },
        { NULL, NULL }
};
