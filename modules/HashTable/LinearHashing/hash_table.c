#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "hash_table.h"

typedef unsigned char uint8_t;

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
    uint64_t curr_size;               // used number of buckets
    uint64_t max_capacity;            // max number of buckets
    uint64_t elements;                // number of elements in the hash table
    uint64_t exponent;                // current explonent (in collaboration with find_func, it is used to find the hash value)
    uint64_t next_split;              // next bucket to be splited
    uint8_t* curr_num_of_elements;    // number of elements in each bucket
    node* buckets;                    // buckets (lists) storing the data
    HashFunc hash;                    // function that hashes an element into a positive integer
    CompareFunc compare;              // function that compares the elements
    DestroyFunc destroy;              // function that destroys the elements, NULL if not
}
hash_table;

#define INCREASE_SIZE 1.8  // increase percentage of maximum number of buckets
#define find_func(i) (1<<i)  // returns 2^i

// Function Prototypes
static inline void hash_resize(const HashTable ht);
static inline uint32_t get_bucket(const HashTable ht, const uint32_t hash_value);
static inline uint64_t hash_search(const HashTable ht, const Pointer value);

HashTable hash_create(const HashFunc hash, const CompareFunc compare, const DestroyFunc destroy)
{
    assert(hash != NULL && compare != NULL);  // a hash and compare function needs to be given
    
    HashTable ht = malloc(sizeof(hash_table));
    assert(ht != NULL);  // allocation failure

    ht->buckets = calloc(sizeof(n), STARTING_HASH_CAPACITY);  // allocate memory for the buckets
    assert(ht->buckets != NULL);  // allocation failure

    ht->curr_num_of_elements = calloc(sizeof(uint8_t), STARTING_HASH_CAPACITY);
    assert(ht->curr_num_of_elements != NULL);  // allocation failure

    ht->max_capacity = STARTING_HASH_CAPACITY;
    ht->elements = 0;
    ht->exponent = 0;
    
    ht->curr_size = 1;
    ht->next_split = 0;

    // initialize functions
    ht->hash = hash;
    ht->compare = compare;
    ht->destroy = destroy;
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
    
    // check to see if value already exists in the hash table
    const uint64_t bucket = hash_search(ht, value);
    if (hash_search(ht, value) == ht->max_capacity)  // value already exists
    {
        // if a destroy function exists, destroy the value
        if (ht->destroy != NULL)
            ht->destroy(value);
        return false;
    }
    
    // create new node
    node new_node = malloc(sizeof(n));
    assert(new_node != NULL);  // allocation failure

    // fill node's contents
    new_node->data = value;
    new_node->hash_value = ht->hash(value);

    // insert value at the start of the bucket
    new_node->next = ht->buckets[bucket];
    ht->buckets[bucket] = new_node;

    ht->curr_num_of_elements[bucket]++;
    
    // split if an overflow occurs
    if (ht->curr_num_of_elements[bucket] > MAX_BUCKET_ELEMENTS)  // max number of elements in the bucket exceeded - overflow
    {
        // get new bucket
        ht->curr_size++;
        
        // maximum capacity of buckets reached, increase size
        if (ht->curr_size == ht->max_capacity)
            hash_resize(ht);
        
        node* new_bucket = &(ht->buckets[ht->curr_size-1]);

        // bucket to be splitted
        node* old_bucket = &(ht->buckets[ht->next_split]);

        ht->next_split++;        
        
        // split operation
        while ((*old_bucket) != NULL)
        {
            if (get_bucket(ht, (*old_bucket)->hash_value) != ht->next_split-1)
            {
                ht->curr_num_of_elements[ht->next_split-1]--;
                ht->curr_num_of_elements[ht->curr_size-1]++;

                *new_bucket = *old_bucket;
                *old_bucket = (*old_bucket)->next;
                new_bucket = &((*new_bucket)->next);
                *new_bucket = NULL;
            }
            else
                old_bucket = &((*old_bucket)->next);
        }

        if (find_func(ht->exponent+1) <= ht->curr_size)
        {
            // start next round of splitting
            ht->exponent++;
            ht->next_split = 0;
        }
    }

    ht->elements++;  // value inserted, increment the number of elements in the hash table
    return true;
}

bool hash_remove(const HashTable ht, const Pointer value)
{
    if (is_ht_empty(ht))  // hash table is empty, nothing to search
        return false;
    
    // find the potential bucket the value belongs to
    const uint64_t h = get_bucket(ht, ht->hash(value));
    node* bkt = &(ht->buckets[h]);
    
    // search for the value at the bucket
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
            
            ht->curr_num_of_elements[h]--;
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
    if (is_ht_empty(ht))  // hash table is empty, nothing to search
        return false;
    
    return ((hash_search(ht, value) == ht->max_capacity) ? true:false);
}

// returns the bucket in which the value exists
// if it does not exist, returns the capacity of the hash table
static inline uint64_t hash_search(const HashTable ht, const Pointer value)
{
    // find the potential bucket the value belongs to
    const uint64_t h = get_bucket(ht, ht->hash(value));

    // search for the value in the bucket
    node bkt = ht->buckets[h];
    while (bkt != NULL)
    {
        Pointer bkt_value = bkt->data;
        if (ht->compare(value, bkt_value) == 0)  // value found
            return ht->max_capacity;
            
        bkt = bkt->next;
    }
    return h;
}

static inline uint32_t get_bucket(const HashTable ht, const uint32_t hash_value)
{
    // use hash function i
    const uint32_t pos = hash_value % find_func(ht->exponent);

    return (pos < ht->next_split? hash_value % find_func(ht->exponent+1):pos);
}

// resize by INCREASE_SIZE
static inline void hash_resize(const HashTable ht)
{
    const uint64_t old_cap = ht->max_capacity;  // old number of buckets
    ht->max_capacity *= INCREASE_SIZE;  // increase capacity
    
    ht->buckets = realloc(ht->buckets, sizeof(*(ht->buckets)) * ht->max_capacity);
    assert(ht->buckets != NULL);  // allocation failure

    ht->curr_num_of_elements = realloc(ht->curr_num_of_elements, sizeof(uint8_t) * ht->max_capacity);
    assert(ht->curr_num_of_elements != NULL);  // allocation failure

    // initialize arrays to avoid errors
    for (uint64_t i = old_cap; i < ht->max_capacity; i++)
    {
        ht->buckets[i] = NULL;
        ht->curr_num_of_elements[i] = 0;
    }
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

    // destroy buckets
    for (uint64_t i = 0; i < ht->max_capacity; i++)
    {
        node bkt = ht->buckets[i];
        while (bkt != NULL)
        {
            node tmp = bkt;
            bkt = bkt->next;

            // if a destroy function exists, destroy the elements
            if (ht->destroy != NULL) 
                ht->destroy(tmp->data);
            free(tmp);
        }
    }
    free(ht->curr_num_of_elements);
    free(ht->buckets);
    free(ht);
}
