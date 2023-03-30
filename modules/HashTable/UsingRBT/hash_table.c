#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hash_table.h"
// red-black tree's include file (note that it might need to be updated according to its path)
#include "../../RedBlackTree/RedBlackTree.h"

#define FIXED_SIZE 3     // minimum number of elements in the array before inserting them at a rbt
#define OVERFLOW_SIZE 4  // size at which elements are inserted at a rbt (FIXED_SIZE+1)

// bucket
typedef struct n
{
    RBTree rbt;        // red-black tree
    Pointer* data;     // array
    uint8_t arr_el;    // number of elements in the array (if it is equal to FIXED_SIZE, the elements are in the rbt)
}
n;

typedef struct hash_table
{
    n* buckets;           // buckets (red-black trees) storing the data
    uint64_t capacity;    // number of buckets
    uint64_t elements;    // number of elements in the hash table
    HashFunc hash;        // function that hashes an element into a positive integer
    CompareFunc compare;  // function that compares the elements
    DestroyFunc destroy;  // function that destroys the elements, NULL if not
}
hash_table;

HashTable hash_create(const HashFunc hash, const CompareFunc compare, const DestroyFunc destroy)
{
    assert(hash != NULL && compare != NULL);  // a hash and compare function needs to be given
    
    HashTable ht = malloc(sizeof(hash_table));
    assert(ht != NULL);  // allocation failure

    ht->capacity = NUM_OF_BUCKETS;

    ht->buckets = calloc(sizeof(n), NUM_OF_BUCKETS);  // allocate memory for the buckets
    assert(ht->buckets != NULL);  // allocation failure

    // allocate memory for the buckets
    for(uint64_t i = 0; i < NUM_OF_BUCKETS; i++)
    {
        ht->buckets[i].data = calloc(sizeof(Pointer), FIXED_SIZE);
        assert(ht->buckets[i].data != NULL);  // allocation failure
    }

    ht->capacity = NUM_OF_BUCKETS;
    ht->elements = 0;
    
    // initialize functions
    ht->compare = compare;
    ht->destroy = destroy;
    ht->hash = hash;
}

uint64_t hash_size(const HashTable ht)
{
    assert(ht != NULL);
    return ht->elements;
}

bool is_ht_empty(const HashTable ht)
{
    assert(ht != NULL);
    return ht->elements == 0;
}

bool hash_insert(const HashTable ht, const Pointer value)
{
    assert(ht != NULL);

    // find the potential bucket the value belongs to
    uint64_t bucket = ht->hash(value) % ht->capacity;
    
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
        for (uint8_t i = 0; i < FIXED_SIZE; i++)
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
            ht->buckets[bucket].rbt = rbt_create(ht->compare, ht->destroy);
            for (uint8_t i = 0; i < FIXED_SIZE; i++)
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

bool hash_remove(const HashTable ht, const Pointer value)
{
    if (is_ht_empty(ht))  // hash table is empty, nothing to search
        return false;
    
    // find the potential bucket the value exists in
    uint64_t bucket = ht->hash(value) % ht->capacity;

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
        uint8_t i = 0;
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

bool hash_exists(const HashTable ht, const Pointer value)
{
    if (is_ht_empty(ht))  // hash table is empty, nothing to search
        return false;
    
    // find the potential bucket the value exists in
    uint64_t bucket = ht->hash(value) % ht->capacity;
    
    // search the rbt
    if (ht->buckets[bucket].arr_el == OVERFLOW_SIZE)
        return rbt_exists(ht->buckets[bucket].rbt, value);
    else  // search the array
    {
        for (uint8_t i = 0; i < FIXED_SIZE; i++)
        {
            if (ht->buckets[bucket].data[i] != NULL && ht->compare(ht->buckets[bucket].data[i], value) == 0)
                return true;  // value found
        }
    }
    return false;  // value not found
}

DestroyFunc hash_set_destroy(const HashTable ht, const DestroyFunc new_destroy_func)
{
    assert(ht != NULL);

    for (uint64_t i = 0; i < ht->capacity; i++)
    {
        if (ht->buckets[i].arr_el == OVERFLOW_SIZE)  // elements are at a rbt in this bucket
            rbt_set_destroy(ht->buckets[i].rbt, new_destroy_func);
    }
    
    DestroyFunc old_destroy_func = ht->destroy;
    ht->destroy = new_destroy_func;
    return old_destroy_func;
}

void hash_destroy(const HashTable ht)
{
    assert(ht != NULL);

    for (uint64_t i = 0; i < ht->capacity; i++)
    {
        if (ht->buckets[i].arr_el != OVERFLOW_SIZE && ht->destroy != NULL)
        {
            for (uint8_t j = 0; j < FIXED_SIZE; j++)
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
