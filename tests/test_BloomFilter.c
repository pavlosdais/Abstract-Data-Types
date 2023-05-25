#include <time.h>
#include "../lib/ADT.h"
#include "./include/common.h"

#define NUM_OF_ELEMENTS 100000
#define BLOOM_SIZE NUM_OF_ELEMENTS*5

void test_create(void)
{
    HashFunc hash_functions[] = {hash_int1, hash_int2, hash_int3};
    bloom_filter bf = bf_create(100000, hash_functions, sizeof(hash_functions)/sizeof(HashFunc));
    TEST_ASSERT(bf != NULL);
    bf_destroy(bf);
}

void test_insert(void)
{
    HashFunc hash_functions[] = {hash_int1, hash_int2, hash_int3};
    bloom_filter bf = bf_create(BLOOM_SIZE, hash_functions, sizeof(hash_functions)/sizeof(HashFunc));

    int* arr = create_ordered_array(NUM_OF_ELEMENTS);
    for (uint32_t i = 0; i < NUM_OF_ELEMENTS; i++)
        bf_insert(bf, &arr[i]);

    unsigned int new_start = NUM_OF_ELEMENTS;
    unsigned int new_end = NUM_OF_ELEMENTS*2;
    int* new_array = create_ordered_array(new_end);

    unsigned int false_positives = 0;
    for (uint32_t i = new_start; i < new_end; i++)
        if (bf_exists(bf, new_array+i))
            false_positives++;

    printf("False positives: %d\n", false_positives);

    // free memory used
    bf_destroy(bf);
    free(arr);
    free(new_array);
}

TEST_LIST = {
        { "create", test_create  },
        { "insert", test_insert  },
        { NULL, NULL }
};
