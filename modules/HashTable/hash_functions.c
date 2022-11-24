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
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c;  // hash*33 + c

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

// source: https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int hash_int(Pointer value)
{
    unsigned int val = (*((int*)value));
    val = ((val >> 16) ^ val) * 0x45d9f3b;
    val = ((val >> 16) ^ val) * 0x45d9f3b;
    val = (val >> 16) ^ val;
    return val;
}
