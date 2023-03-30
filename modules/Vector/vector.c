#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// the starting capacity of the vector
#define STARTING_CAPACITY 8

typedef struct n
{
    Pointer data;
}
n;
typedef struct n* node;

struct vector_struct
{
    node arr;             // array of nodes containing the data
    uint64_t size;        // current size of vector
    uint64_t capacity;    // capacity of the vector
    uint64_t elements;    // current number of elements in the vector
    CompareFunc compare;  // function that compares the elements (used for sort)
    DestroyFunc destroy;  // function that destroys the elements, NULL if not
};

// make sure the index is within the bounds of the vector's array
#define SAFE_INDEX(vector, index) (assert(index >= 0 && index < vector->size))

Vector vector_create(DestroyFunc destroy)
{
    Vector vec = malloc(sizeof(struct vector_struct));
    assert(vec != NULL);  // allocation failure

    vec->arr = calloc(STARTING_CAPACITY, sizeof(*(vec->arr)));
    assert(vec->arr != NULL);  // allocation failure

    // initialize the vector
    vec->destroy = destroy;
    vec->capacity = STARTING_CAPACITY;
    vec->size = vec->elements = 0;
    vec->compare = NULL;
    return vec;
}

uint64_t vector_size(const Vector vector)
{
    assert(vector != NULL);
    return vector->elements;
}

Pointer vector_at(const Vector vector, const uint64_t index)
{
    SAFE_INDEX(vector, index);  // make sure a valid index was given
    return vector->arr[index].data;
}

void vector_set_at(const Vector vector, const uint64_t index, const Pointer data)
{
    SAFE_INDEX(vector, index);  // make sure a valid index was given

    if (vector->arr[index].data != NULL)  // an element already exists there
        if (vector->destroy != NULL)
            vector->destroy(vector->arr[index].data);
    else  // free spot
        vector->elements++;

    vector->arr[index].data = data;
}

bool vector_clear_at(const Vector vector, const uint64_t index)
{
    SAFE_INDEX(vector, index);  // make sure a valid index was given

    // make sure an element exists in the index
    if (vector->arr[index].data != NULL)
    {
        if (vector->destroy != NULL)  // a destroy function exists, clear the data
            vector->destroy(vector->arr[index].data);
        
        vector->arr[index].data = NULL;
        vector->elements--;
        return true;
    }
    return false;
}

void vector_push_back(const Vector vector, const Pointer data)
{
    // array is full, double its size
    if (vector->size == vector->capacity) 
    {
        vector->capacity *= 2;
		vector->arr = realloc(vector->arr,  vector->capacity * sizeof(*(vector->arr)));
    }

    // insert data
    vector->arr[(vector->size)++].data = data;
    vector->elements++;
}

// swaps the value of node a & b
static inline void swap_nodes(const node a, const node b)
{
    node tmp = a->data;
    a->data = b->data;
    b->data = tmp;
}

static inline int partition(const Vector vector, const int left, const int right)
{
    const n pivot = vector->arr[right];
    int i = left-1;

    for (int j = left; j < right; j++)
    {
        if (vector->compare(vector->arr[j].data, pivot.data) < 0)
            swap_nodes(&(vector->arr[++i]), &(vector->arr[j]));
    }
    swap_nodes(&(vector->arr[i+1]), &(vector->arr[right]));

    return i+1;
}

static void quicksort(const Vector vector, const int left, const int right)
{
    if (left < right)
    {
        int pi = partition(vector, left, right);
        
        quicksort(vector, left, pi-1);
        quicksort(vector, pi+1, right);
    }
}

void vector_sort(const Vector vector, const CompareFunc compare)
{
    vector->compare = compare;
    quicksort(vector, 0, vector->size-1);
}

bool vector_binary_search(const Vector vector, const Pointer data, const CompareFunc compare)
{
    uint64_t low = 0, high = vector->size-1;
    while (low <= high)
    {
        const uint64_t mid = low + (high-low) / 2;
        const int cmp = compare(vector->arr[mid].data, data);

        if (cmp == 0)  // found the element
            return true;
        else if (cmp > 0)
            high = mid-1;
        else
            low = mid+1;
    }
    return false;
}

bool vector_search(const Vector vector, const Pointer data, const CompareFunc compare)
{
    uint64_t num_of_elements = vector->elements;
    for (uint64_t element_ind = 0 ;; element_ind++)
    {
        if (vector->arr[element_ind].data != NULL)
        {
            if (compare(vector->arr[element_ind].data, data) == 0) return true;  // data found
            if ((--num_of_elements) == 0) return false;
        }
    }
}

void vector_destroy(const Vector vector)
{
    // first destroy the data, if a destroy function was given
    if (vector->destroy != NULL)
        for (uint64_t element_ind = 0 ;; element_ind++)
        {
            if (vector->arr[element_ind].data != NULL)
            {
                vector->destroy(vector->arr[element_ind].data);
                if ((--(vector->elements)) == 0) break;  // all of the elements are deleted
            }
        }
    
    // destroy the rest of the vector
    free(vector->arr);
    free(vector);
}
