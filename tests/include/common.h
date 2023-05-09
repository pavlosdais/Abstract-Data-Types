#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "acutest.h"

#define calc_time(cur_time) (((double)(clock() - cur_time))/CLOCKS_PER_SEC)

static inline int* allocate_array(unsigned int num_of_elements)
{
    int* arr = malloc(sizeof(int) * num_of_elements);
    assert(arr != NULL);  // allocation failure

    return arr;
}

// creates an ordered array eg. [0, 1, 2.. num_of_elements-1]
static inline int* create_ordered_array(unsigned int num_of_elements)
{
    int* arr = allocate_array(num_of_elements);

    for (unsigned int i = 0; i < num_of_elements; i++)
        arr[i] = i;
    
    return arr;
}

// creates a completely random array (duplicates allowed)
static inline int* create_random_array(unsigned int num_of_elements)
{
    int* arr = allocate_array(num_of_elements);

    for (unsigned int i = 0; i < num_of_elements; i++)
        arr[i] = rand() % RAND_MAX;
    
    return arr;
}

// creates a shuffled random array with elements in range [0, num_of_elements-1]
static inline int* create_shuffled_array(unsigned int num_of_elements)
{
    int* arr = create_ordered_array(num_of_elements);
    
    // shuffle the array
    for (unsigned int i = 0; i < num_of_elements; i++)
    {
        // create random spot for the elements to swap places
        unsigned int new_spot = i + rand() / (RAND_MAX / (num_of_elements-i) + 1);

        int tmp = arr[i];
        arr[i] = arr[new_spot];
        arr[new_spot] = tmp;
    }
    
    return arr;
}

// allocates memory for an integer
int* createData(int a)
{
    int* val = malloc(sizeof(int));
    assert(val != NULL);  // allocation failure

    *val = a;
    return val;
}

// compare function
int compareFunction(void* v1, void* v2)  { return *((int*)v1) - *((int*)v2); }
