#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hash_table.h"

typedef unsigned char sint;

typedef enum
{
    EMPTY, OCCUPIED, DELETED
}
bucket_state;

// bucket
typedef struct n
{
    Pointer data;         // pointer to the data we are storing
    uint32_t hash_value;  // hash value of the data
    bucket_state state;   // state of the bucket (empty/occupied/deleted)
}
n;
typedef struct n* node;

typedef struct hash_table
{
    node buckets;         // buckets storing the data
    uint64_t capacity;    // number of buckets
    uint64_t elements;    // number of elements in the hash table
    HashFunc hash;        // function that hashes an element into a positive integer 
    CompareFunc compare;  // function that compares the elements
    DestroyFunc destroy;  // function that destroys the elements, NULL if not
}
hash_table;


int PRIME_NUM;  // used for the second hash function, it has to be smaller than the hash table's capacity

#define STARTING_HASH_CAPACITY hash_sizes[0]  // starting number of buckets

#define MAX_LOAD_FACTOR 0.5                   // when max load factor is exceeded, rehashing operation occurs


// the second hash function
// source: https://cgi.di.uoa.gr/~k08/manolis/2020-2021/lectures/Hashing.pdf , page 87 
#define hash_func2(PRIME, h1) (PRIME - h1 % PRIME)


// available number of buckets, preferably prime numbers since it has been proven they have better behavior
static uint64_t hash_sizes[] =
    { 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
      786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611,
      402653189, 805306457, 1610612741, 3221225479 };


// function prototype
static inline void rehash(HashTable);

HashTable hash_create(const HashFunc hash, const CompareFunc compare, const DestroyFunc destroy)
{
    assert(hash != NULL && compare != NULL);  // a hash and compare function needs to be given
    
    HashTable ht = malloc(sizeof(hash_table));
    assert(ht != NULL);  // allocation failure

    ht->capacity = STARTING_HASH_CAPACITY;

    ht->buckets = calloc(sizeof(n), ht->capacity);  // allocate memory for the buckets
    assert(ht->buckets != NULL);  // allocation failure

    PRIME_NUM = 27;  // default starting prime
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
    assert(ht != NULL);

    if (((float)ht->elements / ht->capacity) > MAX_LOAD_FACTOR)  // max load factor exceeded, rehash
        rehash(ht);
    
    const uint32_t hash_value = ht->hash(value), interval = hash_func2(PRIME_NUM, hash_value);
    uint32_t pos = hash_value % ht->capacity, pos_adjustment = 0;

    uint64_t deleted_index = ht->capacity+1;  // save deleted node's index if found

    for (uint64_t i = 0; i < ht->capacity; pos_adjustment += interval, i++)
    {
        const uint64_t new_pos = (pos + pos_adjustment) % ht->capacity;

        if (ht->buckets[new_pos].state == EMPTY)  // empty spot found, insert
        {
            pos = deleted_index == ht->capacity+1? new_pos : deleted_index;
            break;
        }
        // a deleted, possible, spot found. Altough we could just insert here, we mark it and keep searching
        // in case the value already exists in order to avoid duplicates
        else if (ht->buckets[new_pos].state == DELETED)
        {
            if (deleted_index == ht->capacity+1)
                deleted_index = new_pos;
        }
        // check to see if value already exists in the hash table
        else if (ht->compare(ht->buckets[new_pos].data, value) == 0)  // value already exists
        {
            // if a destroy function exists, destroy the value
            if (ht->destroy != NULL)
                ht->destroy(value);
            return false;
        }
    }

    ht->buckets[pos].state = OCCUPIED;  // mark the bucket as occupied
    ht->buckets[pos].data = value;
    ht->buckets[pos].hash_value = hash_value;
    ht->elements++;  // value inserted, increment the number of elements in the hash table

    return true;
}

// helper function
static inline void rehash_insert(const HashTable ht, const Pointer value, const uint32_t hash_value);
static inline void rehash(const HashTable ht)
{
    static sint prime_numbers = sizeof(hash_sizes) / sizeof(hash_sizes[0]);
    uint64_t old_capacity = ht->capacity;  // save old number of buckets
    node old_buckets = ht->buckets;  // save previous buckets

    // binary search in array hash_sizes to find the next number of buckets
    sint low = 0, high = prime_numbers-1;
    while (low <= high)
    {
        sint mid = low + (high-low)/2;
        if (old_capacity == hash_sizes[mid])  // found
        {
            PRIME_NUM = old_capacity;
            ht->capacity = (mid+1 == prime_numbers? 2*ht->capacity : hash_sizes[mid+1]);
            
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
        if (old_buckets[i].state == OCCUPIED)
            rehash_insert(ht, old_buckets[i].data, old_buckets[i].hash_value);
    }
    free(old_buckets);
}

static inline void rehash_insert(const HashTable ht, const Pointer value, const uint32_t hash_value)
{
    uint64_t pos = hash_value % ht->capacity, pos_adjustment = 0;
    const uint32_t interval = hash_func2(PRIME_NUM, hash_value);

    for (uint64_t i = 0; i < ht->capacity; pos_adjustment += interval, i++)
    {
        uint64_t new_pos = (pos + pos_adjustment) % ht->capacity;

        // during rehashing only empty spots exist
        if (ht->buckets[new_pos].state == EMPTY)  // empty spot found, insert
        {
            pos = new_pos;
            break;
        }
    }

    ht->buckets[pos].state = OCCUPIED;  // mark the bucket as occupied
    ht->buckets[pos].data = value;
    ht->buckets[pos].hash_value = hash_value;
}

// returns the bucket in which the value exists
// if it does not exist, returns the capacity of the hash table
static inline uint64_t find_bucket(const HashTable ht, const Pointer value)
{
    const uint32_t h1 = ht->hash(value), interval = hash_func2(PRIME_NUM, h1);
    uint64_t buckets_checked = 0;
    
    for (uint64_t pos = h1 % ht->capacity; ht->buckets[pos].state != EMPTY; pos = (pos + interval) % ht->capacity)
    {
        if (ht->buckets[pos].state == OCCUPIED && ht->compare(ht->buckets[pos].data, value) == 0)
            return pos;
        else if (++buckets_checked == ht->capacity)  // searched all buckets containing data, value does not exist
            return ht->capacity;
    }

    // reached an empty bucket, value does not exist
    return ht->capacity;
}

bool hash_remove(const HashTable ht, const Pointer value)
{
    if (is_ht_empty(ht))  // empty hash table - nothing to remove
        return false;
    
    // find the potential bucket the value exists in
    const uint64_t pos = find_bucket(ht, value);
    if (pos == ht->capacity)  // value does not exist
        return false;

    // destroy the data, if a destroy function is given
    if (ht->destroy != NULL)
        ht->destroy(ht->buckets[pos].data);

    ht->buckets[pos].state = DELETED;  // mark the bucket as deleted
    ht->buckets[pos].data = NULL;
    ht->elements--;  // value removed, decrement the number of elements in the hash table
    return true;
}

bool hash_exists(const HashTable ht, const Pointer value)
{
    if (is_ht_empty(ht))  // hash table is empty, nothing to search
        return false;

    return find_bucket(ht, value) != ht->capacity;
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

    // if a destroy function exists & there are elements, destroy the data
    if (ht->destroy != NULL && ht->elements != 0)
    {
        for (uint64_t i = 0; i < ht->capacity; i++)
        {
            if (ht->buckets[i].data != NULL)
            {
                ht->destroy(ht->buckets[i].data);
                if (--(ht->elements) == 0) break;  // all elements deleted
            }
        }
    }
    
    free(ht->buckets);
    free(ht);
}
