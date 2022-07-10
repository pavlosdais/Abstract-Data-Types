#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hash_table.h"

// bucket
typedef struct n
{
    Pointer data;     // pointer to the data we are storing
    uint hash_value;  // hash value of the data
    struct n* next;   // next element in the bucket (NULL if it's the last)
}
n;
typedef struct n* node;

typedef struct hash_table
{
    node* buckets;        // buckets (lists) storing the data
    uint capacity;        // number of buckets
    uint elements;        // number of elements in the hash table
    HashFunc hash;        // function that hashes an element into a positive integer
    CompareFunc compare;  // function that compares the elements
    DestroyFunc destroy;  // function that destroys the elements, NULL if not
}
hash_table;

#define STARTING_HASH_CAPACITY 53  // starting number of buckets
#define MAX_LOAD_FACTOR 1.2

int hash_sizes[] = { 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
                     786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611,
                     402653189, 805306457, 1610612741, 3221225479 };

// Function Prototype
static void rehash(HashTable ht);

void hash_init(HashTable* ht, HashFunc hash, CompareFunc compare, DestroyFunc destroy)
{
    assert(hash != NULL && compare != NULL);  // a hash and compare function needs to be given
    *ht = malloc(sizeof(hash_table));

    assert(*ht != NULL);  // allocation failure

    (*ht)->buckets = calloc(sizeof(n), STARTING_HASH_CAPACITY);  // allocate memory for the buckets
    
    assert((*ht)->buckets != NULL);  // allocation failure

    (*ht)->elements = 0;
    (*ht)->capacity = STARTING_HASH_CAPACITY;
    (*ht)->hash = hash;
    (*ht)->compare = compare;
    (*ht)->destroy = destroy;
}

static void insert(HashTable ht, Pointer value, uint hash_value)
{
    // create new bucket (list)
    node new_node = malloc(sizeof(n));
    assert(new_node != NULL);  // allocation failure

    // fill the node's contents
    new_node->data = value;
    new_node->hash_value = hash_value;

    // insert the value at the start of the buckets
    uint bucket = hash_value % ht->capacity;
    new_node->next = ht->buckets[bucket];
    ht->buckets[bucket] = new_node;
}

void hash_insert(HashTable ht, Pointer value)
{
    if (((float)ht->elements / ht->capacity) >= MAX_LOAD_FACTOR)  // max load factor exceeded
    {
        uint num_of_sizes = sizeof(hash_sizes) / sizeof(hash_sizes[0]);  // number of (prime) sizes
        if (ht->capacity != hash_sizes[num_of_sizes-1])  // if there is an available new size
            rehash(ht);  // rehash
    }

    insert(ht, value, ht->hash(value));

    ht->elements++;  // value inserted, increment the number of elements in the hash table
}

static void rehash(HashTable ht)
{
    uint prime_numbers = sizeof(hash_sizes) / sizeof(hash_sizes[0]), old_size = ht->capacity;
    int low = 0, high = prime_numbers-1;

    // Find the next, in order, size
    while (low <= high)
    {
        int mid = (high + low) / 2;
        if (old_size == hash_sizes[mid])
        {
            ht->capacity = hash_sizes[mid+1];  // get next size
            break;
        }
        else if (old_size < hash_sizes[mid])
            high = mid-1;
        else
            low = mid+1;
    }
    
    // save previous buckets
    node* old_buckets = ht->buckets;
    
    // create more buckets
    ht->buckets = calloc(sizeof(n), ht->capacity);
    assert(ht->buckets != NULL);  // allocation failure

    for (uint i = 0; i < old_size; i++)
    {
        node bkt = old_buckets[i];
    
        while (bkt != NULL)
        {
            node tmp = bkt;
            bkt = bkt->next;
            
            insert(ht, tmp->data, tmp->hash_value);

            free(tmp);
        }
    }
    free(old_buckets);
}

bool hash_remove(HashTable ht, Pointer value)
{
    // rehash to the find the potential buckets the value belongs to
    uint h = ht->hash(value) % ht->capacity;

    node* bkt = &(ht->buckets[h]);
    
    // search the buckets for the value
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
        else  // search next value
            bkt = &((*bkt)->next);
    }
    return false;
}

bool hash_exists(HashTable ht, Pointer value)
{
    // rehash to the find the potential bucket the value belongs to
    uint h = ht->hash(value) % ht->capacity;
    node bkt = ht->buckets[h];
    
    // search for the value in the bucket h
    while (bkt != NULL)
    {
        Pointer bkt_value = bkt->data;
        if (ht->compare(value, bkt_value) == 0)  // value found
            return true;
            
        bkt = bkt->next;
    }
    return false;
}

uint hash_size(HashTable ht)
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
    for (uint i = 0; i < ht->capacity; i++)
    {
        node bkt = ht->buckets[i];
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
    free(ht->buckets);
    free(ht);
}
