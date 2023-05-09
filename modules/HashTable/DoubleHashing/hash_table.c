#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hash_table.h"

// when max load factor is exceeded, rehashing operation occurs
#define MAX_LOAD_FACTOR 0.5

typedef enum { EMPTY = 0, OCCUPIED, DELETED } bucket_state;

// bucket
typedef struct node
{
    Pointer data;         // pointer to the data we are storing
    uint32_t hash_value;  // hash value of the data
    bucket_state state;   // state of the bucket (empty/occupied/deleted)
}
node;

typedef struct hash_table
{
    node* buckets;        // buckets storing the data
    uint8_t capacity;     // the capacity of the hash table - index to the "hash_sizes" array
    uint8_t sec_prime;    // the prime number used for the second hash function
    uint64_t elements;    // number of elements currently stored in the hash table
    HashFunc hash;        // function that hashes an element into a positive integer 
    CompareFunc compare;  // function that compares the elements
    DestroyFunc destroy;  // function that destroys the elements, NULL if not
}
hash_table;

// the second hash function
// source: https://cgi.di.uoa.gr/~k08/manolis/2020-2021/lectures/Hashing.pdf , page 87 
#define hash_func2(PRIME, h1) (PRIME - (h1 % PRIME))

// available number of buckets, preferably prime numbers since it has been proven they have better behavior
static uint64_t hash_sizes[] =
    { 29, 67, 131, 263, 509, 1021, 2053, 4093, 8179, 16369, 32749, 65521, 131071, 262147, 524287, 1048573, 2097143,
    4194301, 8388593, 16777213, 33554467, 67108879, 134217757, 268435459, 536870923, 1073741827, 2147483647, 4294967291,
    8589934583, 17179869143, 34359738337, 68719476731, 137438953447, 274877906899, 549755813881, 1099511627689, 2199023255531,
    4398046511093, 8796093022151, 17592186044399, 35184372088777, 70368744177643, 140737488355213, 281474976710597, 562949953421231,
    1125899906842597, 2251799813685119, 4503599627370449, 9007199254740881, 18014398509481951, 36028797018963913, 72057594037927931,
    144115188075855859, 288230376151711717, 576460752303423433, 1152921504606846883, 2305843009213693951, 4611686018427387847 };

#define get_hash(i) (hash_sizes[i])


// function prototype
static inline void rehash(HashTable);

HashTable hash_create(const HashFunc hash, const CompareFunc compare, const DestroyFunc destroy)
{
    assert(hash != NULL && compare != NULL);  // a hash and compare function needs to be given
    
    HashTable ht = malloc(sizeof(hash_table));
    assert(ht != NULL);  // allocation failure

    ht->capacity = 1;
    ht->buckets = calloc(sizeof(node), get_hash(ht->capacity));  // allocate memory for the buckets
    assert(ht->buckets != NULL);  // allocation failure
    
    ht->sec_prime = 0;
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

    if (((float)ht->elements / get_hash(ht->capacity)) > MAX_LOAD_FACTOR)  // max load factor exceeded, rehash
        rehash(ht);
    
    const uint32_t hash_value = ht->hash(value), interval = hash_func2(get_hash(ht->sec_prime), hash_value);
    uint32_t pos = hash_value % get_hash(ht->capacity), pos_adjustment = 0;

    uint64_t deleted_index = get_hash(ht->capacity)+1;  // save deleted node's index if found

    for (uint64_t i = 0; i < get_hash(ht->capacity); pos_adjustment += interval, i++)
    {
        const uint64_t new_pos = (pos + pos_adjustment) % get_hash(ht->capacity);

        if (ht->buckets[new_pos].state == EMPTY)  // empty spot found, insert
        {
            pos = deleted_index == get_hash(ht->capacity)+1? new_pos : deleted_index;
            break;
        }
        // a deleted, possible, spot found 
        // altough we could just insert here, we mark it and keep searching in case the value already exists in order to avoid duplicates
        else if (ht->buckets[new_pos].state == DELETED)
        {
            if (deleted_index == get_hash(ht->capacity)+1)
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
    // save previous buckets
    node* old_buckets = ht->buckets;

    ht->sec_prime = ht->capacity;
    (ht->capacity)++;
            
    // create the new number of buckets
    ht->buckets = calloc(sizeof(node), get_hash(ht->capacity));
    assert(ht->buckets != NULL);  // allocation failure

    // start rehash operation
    for (uint64_t i = 0; i < get_hash(ht->sec_prime); i++)
    {
        if (old_buckets[i].state == OCCUPIED)
            rehash_insert(ht, old_buckets[i].data, old_buckets[i].hash_value);
    }
    free(old_buckets);
}

static inline void rehash_insert(const HashTable ht, const Pointer value, const uint32_t hash_value)
{
    uint64_t pos = hash_value % get_hash(ht->capacity), pos_adjustment = 0;
    const uint32_t interval = hash_func2(get_hash(ht->sec_prime), hash_value);

    for (uint64_t i = 0; i < get_hash(ht->capacity); pos_adjustment += interval, i++)
    {
        const uint64_t new_pos = (pos + pos_adjustment) % get_hash(ht->capacity);

        // during rehashing only empty spots exist
        if (ht->buckets[new_pos].state == EMPTY)  // empty spot found, insert
        {
            pos = new_pos;
            break;
        }
    }

    // insert the element and mark the bucket as occupied
    ht->buckets[pos].state = OCCUPIED;
    ht->buckets[pos].data = value;
    ht->buckets[pos].hash_value = hash_value;
}

// returns the bucket in which the value exists
// if it does not exist, returns the capacity of the hash table
static inline uint64_t find_bucket(const HashTable ht, const Pointer value)
{
    const uint32_t h1 = ht->hash(value), interval = hash_func2(get_hash(ht->sec_prime), h1);
    uint64_t buckets_checked = 0;
    
    for (uint64_t pos = h1 % get_hash(ht->capacity); ht->buckets[pos].state != EMPTY; pos = (pos + interval) % get_hash(ht->capacity))
    {
        if (ht->buckets[pos].state == OCCUPIED && ht->compare(ht->buckets[pos].data, value) == 0)
            return pos;
        else if (++buckets_checked == get_hash(ht->capacity))  // searched all buckets containing data, value does not exist
            break;
    }

    // reached an empty bucket, value does not exist
    return get_hash(ht->capacity);
}

bool hash_remove(const HashTable ht, const Pointer value)
{
    if (is_ht_empty(ht))  // empty hash table - nothing to remove
        return false;
    
    // find the potential bucket the value exists in
    const uint64_t pos = find_bucket(ht, value);
    if (pos == get_hash(ht->capacity))  // value does not exist
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

    return find_bucket(ht, value) != get_hash(ht->capacity);
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
        for (uint64_t i = 0 ;; i++)
        {
            if (ht->buckets[i].state == OCCUPIED)
            {
                ht->destroy(ht->buckets[i].data);
                if (--(ht->elements) == 0) break;  // all elements deleted
            }
        }
    }
    
    free(ht->buckets);
    free(ht);
}
