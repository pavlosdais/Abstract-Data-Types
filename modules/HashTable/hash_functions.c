#include <string.h>
#include <ctype.h>
#include "hash_functions.h"

// polynomial rolling hash function
// source: https://www.geeksforgeeks.org/string-hashing-using-polynomial-rolling-hash-function/
unsigned int hash_string(Pointer value)
{
    const char* str = (*((const char**)value));
    const int p = 111111, m = 1e9 + 7;
    int hash = 0;
    long p_pow = 1;
    for(int i = 0; str[i] != '\0'; i++)
    {
        hash = (hash + tolower(str[i]) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash;
}

// source: https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int hash_int(Pointer value)
{
    int val = (*((int*)value));
    val = ((val >> 16) ^ val) * 0x45d9f3b;
    val = ((val >> 16) ^ val) * 0x45d9f3b;
    val = (val >> 16) ^ val;
    return val;
}
