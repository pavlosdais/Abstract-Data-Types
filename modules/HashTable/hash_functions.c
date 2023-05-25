#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "hash_functions.h"

// polynomial rolling hash function
// source: https://www.geeksforgeeks.org/string-hashing-using-polynomial-rolling-hash-function/
unsigned int hash_string1(Pointer value)
{
    const char* str = (*((const char**)value));
    
    const int p = 111111, m = 1e9 + 7;
    unsigned int hash = 0;
    unsigned long p_pow = 1;
    for(int i = 0; str[i] != '\0'; i++)
    {
        hash = (hash + tolower(str[i]) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash;
}

// djb2 hash function
// source: http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash_string2(Pointer value)
{
    const char* str = (*((const char**)value));

    unsigned int hash = 5381;
    for(unsigned int i = 0; str[i] != '\0'; i++)
        hash = ((hash << 5) + hash) + str[i];  // hash*33 + c

    return hash;
}

// simple hash function that sums the characters of the string
unsigned int hash_string3(Pointer value)
{
    const char* str = (*((const char**)value));

    unsigned int i, sum = 0;
    for(i = 0; str[i] != '\0'; i++)
        sum += str[i];
    return sum;
}

// FNV-1a algorithm
unsigned int hash_int1(Pointer value)
{
    int val = (*((int*)value));

    unsigned int hash = 2166136261u;  // FNV offset basis
    const unsigned char* data = (unsigned char*)&val;
    for (int i = 0; i < sizeof(int); i++) 
    {
        hash ^= data[i];
        hash *= 16777619;  // FNV prime
    }
    return hash;
}

unsigned int hash_int2(Pointer value)
{
    int val = (*((int*)value));

    val = val ^ (val >> 4);
    val = (val ^ 0xdeadbeef) + (val << 5);
    val = val ^ (val >> 11);
    return (unsigned int)val;
}

unsigned int hash_int3(Pointer value)
{
    unsigned int hash = (unsigned int)(*((int*)value));

    hash = (hash + 0x7ed55d16) + (hash << 12);
    hash = (hash ^ 0xc761c23c) ^ (hash >> 19);
    hash = (hash + 0x165667b1) + (hash << 5);
    hash = (hash + 0xd3a2646c) ^ (hash << 9);
    hash = (hash + 0xfd7046c5) + (hash << 3);
    hash = (hash ^ 0xb55a4f09) ^ (hash >> 16);
    return hash;
}
