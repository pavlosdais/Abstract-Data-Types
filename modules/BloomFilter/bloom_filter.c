#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bloom_filter.h"

typedef struct bfilter
{
    uint8_t* bit_array;  // bit array
    uint32_t size;       // size of the bit array
    HashFunc* hash;      // array of hash functions
    uint8_t hash_count;  // number of hash functions
}
bfilter;

#define create_mask(x) (1 << x)
#define get_index(x) (x / sizeof(uint8_t))
#define get_bit(x) (x % sizeof(uint8_t))

bloom_filter bf_create(const uint32_t max_capacity, HashFunc* hash, const uint8_t hash_count)
{
    assert(hash != NULL);

    bloom_filter bf = malloc(sizeof(bfilter));
    assert(bf != NULL);

    bf->size = hash_count*max_capacity;  // size of the bit array

    bf->bit_array = calloc(sizeof(uint8_t), bf->size/sizeof(uint8_t)+1);
    assert(bf->bit_array != NULL);

    bf->hash_count = hash_count;
    bf->hash = hash;

    return bf;
}

static inline void set_bit(bloom_filter bf, const uint32_t hash_value)
{
    bf->bit_array[get_index(hash_value)] |= create_mask(get_bit(hash_value));
}

static inline bool bit_exists(bloom_filter bf, const uint32_t hash_value)
{
    return bf->bit_array[get_index(hash_value)] & create_mask(get_bit(hash_value)) != 0;
}

void bf_insert(const bloom_filter bf, const Pointer value)
{
    assert(bf != NULL && value != NULL);

    // for every hash function, hash the value and set the bit
    for (uint8_t i = 0; i < bf->hash_count; i++)
        set_bit(bf, bf->hash[i](value) % bf->size);
}

bool bf_exists(const bloom_filter bf, const Pointer value)
{
    assert(bf != NULL && value != NULL);

    // hash the value and check if the bit is set
    for (uint8_t i = 0; i < bf->hash_count; i++)
    {
        if (!bit_exists(bf, bf->hash[i](value) % bf->size))
            return false;
    }

    return true;
}

void bf_destroy(bloom_filter bf)
{
    assert(bf != NULL);

    free(bf->bit_array);
    free(bf);
}
