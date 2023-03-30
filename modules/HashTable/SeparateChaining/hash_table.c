#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hash_table.h"

typedef unsigned char sint;

// bucket
typedef struct n
{
    Pointer data;         // pointer to the data we are storing
    uint32_t hash_value;  // hash value of the data
    struct n* next;       // next element in the bucket (NULL if it's the last)
}
n;
typedef struct n* node;

typedef struct hash_table
{
    node* buckets;        // buckets (lists) storing the data
    uint64_t capacity;    // number of buckets
    uint64_t elements;    // number of elements in the hash table
    HashFunc hash;        // function that hashes an element into a positive integer
    CompareFunc compare;  // function that compares the elements
    DestroyFunc destroy;  // function that destroys the elements, NULL if not
}
hash_table;

#define STARTING_HASH_CAPACITY hash_sizes[0]  // starting number of buckets


// available number of buckets, preferably prime numbers since it has been proven they have better behavior
static uint64_t hash_sizes[] =
    { 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
      786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611,
      402653189, 805306457, 1610612741, 3221225479 };

// function prototypes
static inline void insert(const HashTable ht, const Pointer value, const uint32_t hash_value);
static inline void rehash(const HashTable ht);
static inline bool hash_search(const HashTable ht, const Pointer value, uint32_t* hash_value);

HashTable hash_create(const HashFunc hash, const CompareFunc compare, const DestroyFunc destroy)
{
    assert(hash != NULL && compare != NULL);  // a hash and compare function needs to be given
    
    HashTable ht = malloc(sizeof(hash_table));
    assert(ht != NULL);  // allocation failure

    ht->capacity = STARTING_HASH_CAPACITY;

    ht->buckets = calloc(sizeof(n), ht->capacity);  // allocate memory for the buckets
    assert(ht->buckets != NULL);  // allocation failure

    ht->elements = 0;
    ht->hash = hash;
    ht->compare = compare;
    ht->destroy = destroy;

    return ht;
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
    // check to see if value already exists in the hash table
    uint32_t hash_value;
    if (hash_search(ht, value, &hash_value))  // value already exists
    {
        if (ht->destroy != NULL)
            ht->destroy(value);
        return false;
    }

    if (((float)ht->elements / ht->capacity) > MAX_LOAD_FACTOR)  // max load factor exceeded, try to rehash
    {
        if (ht->capacity != hash_sizes[sizeof(hash_sizes) / sizeof(hash_sizes[0]) - 1])  // if a new, available, size exists
            rehash(ht);  // rehash
    }
    
    // insert value
    insert(ht, value, hash_value);

    ht->elements++;  // value inserted, increment the number of elements in the hash table
    return true;
}

static inline void insert(const HashTable ht, const Pointer value, const uint32_t hash_value)
{
    // create new bucket (list)
    node new_node = malloc(sizeof(n));
    assert(new_node != NULL);  // allocation failure

    // fill the node's contents
    new_node->data = value;
    new_node->hash_value = hash_value;

    // insert value at the start of the bucket
    uint32_t bucket = hash_value % ht->capacity;
    new_node->next = ht->buckets[bucket];
    ht->buckets[bucket] = new_node;
}

static inline void rehash(HashTable ht)
{
    uint64_t old_capacity = ht->capacity;  // save old number of buckets
    node* old_buckets = ht->buckets;  // save previous buckets

    // binary search in array hash_sizes to find the next number of buckets
    sint low = 0, high = sizeof(hash_sizes) / sizeof(hash_sizes[0]) - 1;
    while (low <= high)
    {
        sint mid = low + (high-low)/2;
        if (old_capacity == hash_sizes[mid])  // found
        {
            ht->capacity = hash_sizes[mid+1];  // get next size

            // create the new number of buckets
            ht->buckets = calloc(sizeof(n), ht->capacity);
            assert(ht->buckets != NULL);  // allocation failure

            break;
        }
        else if (old_capacity < hash_sizes[mid])
            high = mid-1;
        else
            low = mid+1;
    }

    // start rehash operation
    for (uint64_t i = 0; i < old_capacity; i++)
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

bool hash_remove(const HashTable ht, const Pointer value)
{
    if (is_ht_empty(ht))  // hash table is empty, nothing to search
        return false;
    
    // hash to the find the potential bucket the value belongs to
    uint32_t hash_value = ht->hash(value) % ht->capacity;

    node* bkt = &(ht->buckets[hash_value]);
    
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
        else  // search next value
            bkt = &((*bkt)->next);
    }
    return false;
}

bool hash_exists(const HashTable ht, const Pointer value)
{
    uint32_t tmp = ht->hash(value);  // hash the value

    return hash_search(ht, value, &tmp);
}

static inline bool hash_search(const HashTable ht, const Pointer value, uint32_t* hash_value)
{
    *hash_value = ht->hash(value);
    if (is_ht_empty(ht))  // hash table is empty, nothing to search
        return false;
    
    node bkt = ht->buckets[*hash_value % ht->capacity];
    
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

DestroyFunc hash_set_destroy(const HashTable ht, const DestroyFunc new_destroy_func)
{
    assert(ht != NULL);

    DestroyFunc old_destroy_func = ht->destroy;
    ht->destroy = new_destroy_func;
    return old_destroy_func;
}

void hash_destroy(const HashTable ht)
{
    assert(ht != NULL);

    // destroy the buckets
    if (ht->elements != 0)
    {
        for (uint64_t i = 0; i < ht->capacity; i++)
        {
            node bkt = ht->buckets[i];
            while (bkt != NULL)
            {
                node tmp = bkt;
                bkt = bkt->next;

                // if a destroy function exists, destroy the data
                if (ht->destroy != NULL) 
                    ht->destroy(tmp->data);
                free(tmp);
                --(ht->elements);
            }
            if (ht->elements == 0) break;  // all elements deleted
        }
    }
    
    free(ht->buckets);
    free(ht);
}
