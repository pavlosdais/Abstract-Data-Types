#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hash_table.h"

typedef unsigned int natural_num;

// bucket
typedef struct n
{
    Pointer data;
    struct n *next;  // next element in the bucket (NULL if it's the last)
}
n;
typedef struct n* node;

typedef struct hash_table
{
    node* bucket;          // buckets (lists) storing the data
    natural_num capacity;  // number of buckets
    natural_num elements;  // number of elements in the hash table
    HashFunc hash;         // function that hashes the elements into positive integers
    CompareFunc compare;   // function that compares the elements
    DestroyFunc destroy;   // function that destroys the elements, NULL if not
}
hash_table;

void hash_init(HashTable* ht, HashFunc hash, CompareFunc compare, DestroyFunc destroy)
{
    assert(hash != NULL && compare != NULL);  // a hash and compare function needs to be given
    *ht = malloc(sizeof(hash_table));

    assert(*ht != NULL);  // allocation failure

    (*ht)->bucket = calloc(sizeof(n), HASH_CAPACITY);  // allocate memory for the buckets
    
    assert((*ht)->bucket != NULL);  // allocation failure

    (*ht)->capacity = HASH_CAPACITY;
    (*ht)->hash = hash;
    (*ht)->compare = compare;
    (*ht)->destroy = destroy;
}

void hash_insert(HashTable ht, Pointer value)
{
    // create new bucket (list)
    node new_node = malloc(sizeof(n));
    assert(new_node != NULL);  // allocation failure
    new_node->data = value;

    // find the bucket the value should be inserted into
    natural_num h = ht->hash(value) % ht->capacity;

    // insert the value at the start of the bucket
    new_node->next = ht->bucket[h];
    ht->bucket[h] = new_node;

    ht->elements++;  // value inserted, increment the number of elements in the hash table
}

bool hash_remove(HashTable ht, Pointer value)
{
    // rehash to the find the potential bucket the value belongs to
    natural_num h = ht->hash(value) % ht->capacity;

    node* bkt = &(ht->bucket[h]);
    
    // search the bucket for the value
    while (*bkt != NULL)
    {
        Pointer bkt_value = (*bkt)->data;
        
        if (ht->compare(value, bkt_value) == 0)  // value found
        {
            node tmp = *bkt;
            (*bkt) = (*bkt)->next;

            // if a destroy function exists, destroy the value
            if (ht->destroy != NULL)
                ht->destroy(tmp->data);
            
            free(tmp);
            ht->elements--;  // value removed, decrement the number of elements in the hash table
            return true;
        }
        else bkt = &((*bkt)->next);
    }
    return false;
}

bool hash_exists(HashTable ht, Pointer value)
{
    // rehash to the find the potential bucket the value belongs to
    natural_num h = ht->hash(value) % ht->capacity;
    node bkt = ht->bucket[h];
    
    while (bkt != NULL)
    {
        Pointer bkt_value = bkt->data;
        if (ht->compare(value, bkt_value) == 0)  // value found
            return true;
            
        bkt = bkt->next;
    }
    return false;
}

unsigned int hash_size(HashTable ht)
{
    return ht->elements;
}

DestroyFunc hash_set_destroy(HashTable ht, DestroyFunc new_destroy_func)
{
    DestroyFunc old_destroy_func = ht->destroy;
    ht->destroy = new_destroy_func;
    return old_destroy_func;
}

void hash_destroy(HashTable ht)
{
    // destroy buckets
    for (natural_num i = 0; i < ht->capacity; i++)
    {
        node bkt = ht->bucket[i];
        while (bkt != NULL)
        {
            node tmp = bkt;
            bkt = bkt->next;

            // if a destroy function is given, destroy the elements
            if (ht->destroy != NULL) 
                ht->destroy(tmp->data);
            free(tmp);
        }
    }
    free(ht->bucket);
    free(ht);
}
