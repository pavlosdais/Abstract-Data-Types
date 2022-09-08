#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hash_table.h"
// red-black tree's include file (note that it might need to be updated according to its path)
#include "../../RedBlackTree/RedBlackTree.h"

#define FIXED_SIZE 3     // minimum number of elements in the array before inserting them at a rbt
#define OVERFLOW_SIZE 4  // size at which elements are inserted at a rbt (FIXED_SIZE+1)

typedef unsigned char small_int;

// bucket
typedef struct n
{
    RBTree rbt;        // red-black tree
    Pointer* data;     // array
    small_int arr_el;  // number of elements in the array (if it is equal to FIXED_SIZE, the elements are in the rbt)
}
n;

typedef struct hash_table
{
    n* buckets;           // buckets (red-black trees) storing the data
    uint capacity;        // number of buckets
    uint elements;        // number of elements in the hash table
    HashFunc hash;        // function that hashes an element into a positive integer
    CompareFunc compare;  // function that compares the elements
    DestroyFunc destroy;  // function that destroys the elements, NULL if not
}
hash_table;

void hash_init(HashTable* ht, HashFunc hash, CompareFunc compare, DestroyFunc destroy)
{
    assert(hash != NULL && compare != NULL);  // a hash and compare function needs to be given
    
    *ht = malloc(sizeof(hash_table));
    assert(*ht != NULL);  // allocation failure

    (*ht)->capacity = NUM_OF_BUCKETS;

    (*ht)->buckets = calloc(sizeof(n), (*ht)->capacity);  // allocate memory for the buckets
    assert((*ht)->buckets != NULL);  // allocation failure

    // allocate memory for the buckets
    for(uint i = 0; i < (*ht)->capacity; i++)
    {
        (*ht)->buckets[i].data = calloc(sizeof(Pointer), FIXED_SIZE);
        assert((*ht)->buckets[i].data != NULL);  // allocation failure
    }

    (*ht)->elements = 0;
    
    // initialize functions
    (*ht)->compare = compare;
    (*ht)->destroy = destroy;
    (*ht)->hash = hash;
}

uint hash_size(HashTable ht)
{
    assert(ht != NULL);
    return ht->elements;
}

bool is_ht_empty(HashTable ht)
{
    assert(ht != NULL);
    return ht->elements == 0;
}

bool hash_insert(HashTable ht, Pointer value)
{
    assert(ht != NULL);

    // find the potential bucket the value belongs to
    uint bucket = ht->hash(value) % ht->capacity;
    
    // insert at the rbt
    if (ht->buckets[bucket].arr_el == OVERFLOW_SIZE)
    {
        if (rbt_insert(ht->buckets[bucket].rbt, value))
        {
            ht->elements++;  // value inserted, increment the number of elements in the hash table
            return true;
        }
    }
    else  // insert at the array
    {
        // search to see if value already exists
        int empty_space = -1;
        for (small_int i = 0; i < FIXED_SIZE; i++)
        {
            if (ht->buckets[bucket].data[i] == NULL)
            {
                if (empty_space == -1)
                    empty_space = i;
            }
            else if (ht->compare(ht->buckets[bucket].data[i], value) == 0)  // value already exists
            {
                // if a destroy function exists, destroy the value
                if (ht->destroy != NULL)
                    ht->destroy(value);
                return false;
            }
        }

        // value does not already exist, insert operation
        ht->buckets[bucket].arr_el++;

        if (ht->buckets[bucket].arr_el == OVERFLOW_SIZE)  // overflow
        {
            // move all the elemenets to a rbt
            rbt_init(&(ht->buckets[bucket].rbt), ht->compare, ht->destroy);
            for (small_int i = 0; i < FIXED_SIZE; i++)
            {
                if (ht->buckets[bucket].data[i] != NULL)
                    rbt_insert(ht->buckets[bucket].rbt, ht->buckets[bucket].data[i]);
            }
            rbt_insert(ht->buckets[bucket].rbt, value);

            // data has now been moved to a rbt, no need for the array anymore
            free(ht->buckets[bucket].data);
        }
        else
            ht->buckets[bucket].data[empty_space] = value;
        
        ht->elements++;  // value inserted, increment the number of elements in the hash table
        return true;
    }

    // value already exists in the hash table
    return false;
}

bool hash_remove(HashTable ht, Pointer value)
{
    if (is_ht_empty(ht))  // hash table is empty, nothing to search
        return false;
    
    // find the potential bucket the value exists in
    uint bucket = ht->hash(value) % ht->capacity;

    if (ht->buckets[bucket].arr_el == OVERFLOW_SIZE)
    {
        if (rbt_remove(ht->buckets[bucket].rbt, value))
        {
            ht->elements--;  // value removed, decrement the number of elements in the hash table
            return true;
        }
    }
    else
    {
        // search for the value
        bool found = false;
        small_int i = 0;
        for (i = 0; i < FIXED_SIZE; i++)
        {
            if (ht->buckets[bucket].data[i] != NULL && ht->compare(ht->buckets[bucket].data[i], value) == 0)
            {
                found = true;
                break;
            }
        }
        if (found)  // value found
        {
            // if a destroy function exists, destroy the value
            if (ht->destroy != NULL)
                ht->destroy(ht->buckets[bucket].data[i]);
            
            ht->buckets[bucket].data[i] = NULL;  // mark the spot empty
            ht->buckets[bucket].arr_el--;
            ht->elements--;  // value removed, decrement the number of elements in the hash table
            return true;
        }
    }
    
    // value does not exist in the hash table
    return false;
}

bool hash_exists(HashTable ht, Pointer value)
{
    if (is_ht_empty(ht))  // hash table is empty, nothing to search
        return false;
    
    // find the potential bucket the value exists in
    uint bucket = ht->hash(value) % ht->capacity;
    
    // search the rbt
    if (ht->buckets[bucket].arr_el == OVERFLOW_SIZE)
        return rbt_exists(ht->buckets[bucket].rbt, value);
    else  // search the array
    {
        for (small_int i = 0; i < FIXED_SIZE; i++)
        {
            if (ht->buckets[bucket].data[i] != NULL && ht->compare(ht->buckets[bucket].data[i], value) == 0)
                return true;  // value found
        }
    }
    return false;  // value not found
}

DestroyFunc hash_set_destroy(HashTable ht, DestroyFunc new_destroy_func)
{
    assert(ht != NULL);

    for (uint i = 0; i < ht->capacity; i++)
    {
        if (ht->buckets[i].arr_el == OVERFLOW_SIZE)  // elements are at a rbt in this bucket
            rbt_set_destroy(ht->buckets[i].rbt, new_destroy_func);
    }
    
    DestroyFunc old_destroy_func = ht->destroy;
    ht->destroy = new_destroy_func;
    return old_destroy_func;
}

void hash_destroy(HashTable ht)
{
    assert(ht != NULL);

    for (uint i = 0; i < ht->capacity; i++)
    {
        if (ht->buckets[i].arr_el != OVERFLOW_SIZE && ht->destroy != NULL)
        {
            for (small_int j = 0; j < FIXED_SIZE; j++)
            {
                if (ht->buckets[i].data[j] != NULL)
                    ht->destroy(ht->buckets[i].data[j]);
            }
        }
        if (ht->buckets[i].arr_el == OVERFLOW_SIZE)  // elements are at a rbt
            rbt_destroy(ht->buckets[i].rbt);
        else  // elements are at an array
            free(ht->buckets[i].data);
    }
    free(ht->buckets);
    free(ht);
}
