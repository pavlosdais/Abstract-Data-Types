# Abstract Data Types in C
This is a modern C library containing implementations designed to provide fast and efficient data structure operations for a wide range of applications whilst also being educational, having an easy to understand code. All ADT's have been implemented using [void pointers](https://www.geeksforgeeks.org/void-pointer-c-cpp/) to make them generic, allowing the same code to handle different data types (eg integers, strings, structs etc). The library contains the following ADT's:

* [Vector](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules/Vector#readme)
* [Stack](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules/Stack#readme)
* [Queue](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules/Queue#readme)
* [Priority Queue](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules/PriorityQueue#readme)
* [Red-Black Tree](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules/RedBlackTree#readme)
* [Hash Table](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules/HashTable#readme)
* [Graph](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules/Graph#readme)

The source code of every ADT can be found over at the [modules](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules) directory.

# Usage
The library can be used in your C project by following 3 (simple) steps.

### Step 1
Include the header file of the ADT library
```c
#include "ADT.h"
```
*or* just include the desired header files of the data structures you want to use.
```c
#include "vector.h"
#include "RedBlackTree.h"
#include "stack.h"
#include "pq.h"
#include "hash_table.h"
```

### Step 2
Depending on the ADT, provide a number of the following functions upon initialization:
- **Destroy function** destroys the data (or NULL if you do not want the data to be deleted).
```c
void free(void* ptr);  // function provided by <stdio.h> that deallocates memory previously allocated by ptr.
```

- **Compare function** compares two Pointers a and b, and returns -1 if a < b, 0 if a = b or 1 if a > b.
```c
int compareFunction(Pointer v1, Pointer v2)  { return *((int*)v1) - *((int*)v2); }
```

- **Hash function** used by the hash table, takes a Pointer(our data) and returns a positive integer.
```c
unsigned int hash_int(Pointer value)
{
    unsigned int val = (*((int*)value));
    val = ((val >> 16) ^ val) * 0x45d9f3b;
    val = ((val >> 16) ^ val) * 0x45d9f3b;
    val = (val >> 16) ^ val;
    return val;
}
```

- **Visit function** used by the graph, visits a vertex.
```c
void visit(Vertex x)  { printf("%d ", x); }
```

### Step 3
Use `ADTlib.a` on compilation.
```bash
~$ gcc -o my_prog_exec my_prog.c -L. lib/ADTlib.a
```

# Examples
The library can be recompiled by running `make`. Check the [tests](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/tests) directory for some detailed examples on its usage.

---
© Pavlos Dais