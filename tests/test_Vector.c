#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "../lib/ADT.h"

// function prototypes
int* createData(int a);
void printFunc(Pointer value);
int compareFunction(Pointer v1, Pointer v2);

#define NUM_OF_ELEMENTS 1000

int main(void)
{
    Vector vec = vector_create(free);

    int* arr = malloc(sizeof(int) * NUM_OF_ELEMENTS);
    assert(arr != NULL);  // allocation failure

    time_t t;
    srand((unsigned) time(&t));
    for (int i = 0; i < NUM_OF_ELEMENTS; i++)
        arr[i] = rand() % RAND_MAX;

    for (int i = 0; i < NUM_OF_ELEMENTS; i++)
        vector_push_back(vec, createData(arr[i]));

    if (vector_size(vec) != NUM_OF_ELEMENTS)
        printf("Error in size/push_back\n");
    
    // test linear search
    int num_of_elements = 0;
    for (int i = 0; i < NUM_OF_ELEMENTS; i++)
        if (vector_search(vec, arr + i, compareFunction))
            num_of_elements++;
    
    if (num_of_elements != NUM_OF_ELEMENTS)
        printf("Error in linear search\n");

    // sort the vector
    vector_sort(vec, compareFunction);

    // test binary search
    num_of_elements = 0;
    for (int i = 0; i < NUM_OF_ELEMENTS; i++)
        if (vector_binary_search(vec, arr + i, compareFunction))
            num_of_elements++;
    
    if (num_of_elements != NUM_OF_ELEMENTS)
        printf("Error in binary search\n");

    free(arr);
    vector_destroy(vec);
}

int* createData(int a)
{
    int* val = malloc(sizeof(int));
    assert(val != NULL);  // allocation failure

    *val = a;
    return val;
}

// print function
void printFunc(Pointer value)
{
    int* val = (int*)(value);
    printf("%d ", *val);
}

// compare function (for sorted insert)
int compareFunction(Pointer v1, Pointer v2)
{
    return *((int*)v1) - *((int*)v2);
}