#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hash_table.h"

// when max load factor is exceeded, rehashing operation occurs
#define MAX_LOAD_FACTOR 0.8

// bucket
typedef struct node
{
    Pointer data;         // pointer to the data we are storing
    uint32_t hash_value;  // hash value of the data
    struct node* next;    // next element in the bucket (NULL if it's the last)
}
node;

typedef struct hash_table
{
    node** buckets;       // buckets (lists) storing the data
    uint8_t capacity;     // the capacity of the hash table - index to the "hash_sizes" array
    uint64_t elements;    // number of elements in the hash table
    HashFunc hash;        // function that hashes an element into a positive integer
    CompareFunc compare;  // function that compares the elements
    DestroyFunc destroy;  // function that destroys the elements, NULL if not
}
hash_table;

// available number of buckets, preferably prime numbers since it has been proven they have better behavior
static uint64_t hash_sizes[] =
    { 67, 131, 263, 509, 1021, 2053, 4093, 8179, 16369, 32749, 65521, 131071, 262147, 524287, 1048573, 2097143,
    4194301, 8388593, 16777213, 33554467, 67108879, 134217757, 268435459, 536870923, 1073741827, 2147483647, 4294967291,
    8589934583, 17179869143, 34359738337, 68719476731, 137438953447, 274877906899, 549755813881, 1099511627689, 2199023255531,
    4398046511093, 8796093022151, 17592186044399, 35184372088777, 70368744177643, 140737488355213, 281474976710597, 562949953421231,
    1125899906842597, 2251799813685119, 4503599627370449, 9007199254740881, 18014398509481951, 36028797018963913, 72057594037927931,
    144115188075855859, 288230376151711717, 576460752303423433, 1152921504606846883, 2305843009213693951, 4611686018427387847 };

#define get_hash(i) (hash_sizes[i])

// function prototypes
static inline void insert(const HashTable ht, const Pointer value, const uint32_t hash_value);
static inline void rehash(const HashTable ht);
static inline bool hash_search(const HashTable ht, const Pointer value, uint32_t* hash_value);

HashTable hash_create(const HashFunc hash, const CompareFunc compare, const DestroyFunc destroy)
{
    assert(hash != NULL && compare != NULL);  // a hash and compare function needs to be given
    
    HashTable ht = malloc(sizeof(hash_table));
    assert(ht != NULL);  // allocation failure

    ht->capacity = 0;
    ht->buckets = calloc(sizeof(node), get_hash(ht->capacity));  // allocate memory for the buckets
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
    uint32_t hash_value = 0;
    if (hash_search(ht, value, &hash_value))  // value already exists
    {
        if (ht->destroy != NULL) ht->destroy(value);
        return false;
    }

    if (((float)ht->elements / get_hash(ht->capacity)) > MAX_LOAD_FACTOR)  // max load factor exceeded, try to rehash
    {
        if (get_hash(ht->capacity) != hash_sizes[sizeof(hash_sizes) / sizeof(hash_sizes[0]) - 1])  // if a new, available, size exists
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
    node* new_node = malloc(sizeof(node));
    assert(new_node != NULL);  // allocation failure

    // fill the node's contents
    new_node->data = value;
    new_node->hash_value = hash_value;

    // insert value at the start of the bucket
    uint32_t bucket = hash_value % get_hash(ht->capacity);
    new_node->next = ht->buckets[bucket];
    ht->buckets[bucket] = new_node;
}

static inline void rehash(HashTable ht)
{
    node** old_buckets = ht->buckets;  // save previous buckets

    uint8_t old_capacity = ht->capacity;
    (ht->capacity)++;  // get next size

    // create the new number of buckets
    ht->buckets = calloc(sizeof(node), get_hash(ht->capacity));
    assert(ht->buckets != NULL);  // allocation failure

    // start rehash operation
    for (uint64_t i = 0; i < get_hash(old_capacity); i++)
    {
        node* bkt = old_buckets[i];
    
        while (bkt != NULL)
        {
            node* tmp = bkt;
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
    uint32_t hash_value = ht->hash(value) % get_hash(ht->capacity);

    node** bkt = &(ht->buckets[hash_value]);
    
    // search the bucket for the value
    while (*bkt != NULL)
    {
        Pointer bkt_value = (*bkt)->data;
        
        if (ht->compare(value, bkt_value) == 0)  // value found
        {
            node* tmp = *bkt;
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
    uint32_t tmp = 0;
    return hash_search(ht, value, &tmp);
}

static inline bool hash_search(const HashTable ht, const Pointer value, uint32_t* hash_value)
{
    *hash_value = ht->hash(value);
    if (is_ht_empty(ht))  // hash table is empty, nothing to search
        return false;
    
    node* bkt = ht->buckets[*hash_value % get_hash(ht->capacity)];
    
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
        for (uint64_t i = 0 ;; i++)
        {
            node* bkt = ht->buckets[i];
            while (bkt != NULL)
            {
                node* tmp = bkt;
                bkt = bkt->next;

                // if a destroy function exists, destroy the data
                if (ht->destroy != NULL)  ht->destroy(tmp->data);
                
                free(tmp);
                --(ht->elements);
            }
            if (ht->elements == 0) break;  // all elements deleted
        }
    }
    
    free(ht->buckets);
    free(ht);
}
