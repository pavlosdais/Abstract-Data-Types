#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef void* Pointer;

// Pointer to function that hashes a value to a positive (unsigned) integer
typedef unsigned int (*HashFunc)(Pointer value);

typedef struct bfilter* bloom_filter;

// creates bloom filter
// -requires an array of hash functions
bloom_filter bf_create(const uint32_t, HashFunc*, const uint8_t);

// inserts value at the bloom filter
void bf_insert(const bloom_filter, const Pointer);

// returns true if value exists in the bloom filter, false otherwise
// false positives are possible, but false negatives are not
bool bf_exists(const bloom_filter, const Pointer);

// destroys the memory used by the bloom filter
void bf_destroy(bloom_filter);
