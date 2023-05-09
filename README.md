# Abstract Data Types in C
This is a modern C library containing implementations designed to provide fast and efficient data structure operations for a wide range of applications whilst also being educational, having an relatively non-challenging code to follow. All ADT's have been implemented using [void pointers](https://www.geeksforgeeks.org/void-pointer-c-cpp/) to make them generic, allowing the same code to handle different data types (eg integers, strings, structs etc). The library contains the following ADT's:

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
#include "lib/ADT.h"
```
*or* just include the desired header file(s) of the data structure(s) you want to use.
```c
#include "vector.h"
#include "RedBlackTree.h"
#include "stack.h"
#include "pq.h"
#include "hash_table.h"
```

### Step 2
Depending on the ADT, provide a number of the following functions upon initialization:
- **Destroy function** <br/>
    Destroys the data allocated for the elements.
```c
void free(void* ptr);  // function provided by <stdio.h> that deallocates memory previously allocated by ptr.
```

- **Compare function** <br/>
    Compares two Pointers a and b, and returns < 0 if a < b, 0 if a = b or > 0 if a > b.
```c
int compareFunction(Pointer v1, Pointer v2)  { return *((int*)v1) - *((int*)v2); }
```

- **Hash function** <br/>
    Takes a Pointer and returns a unsigned integer (Used by the hash table).
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

- **Visit function** <br/>
    Visits a vertex (Used by the graph).
```c
void visit(Vertex x)  { printf("%d ", x); }
```

### Step 3
Use `ADTlib.a` on compilation.
```bash
~$ gcc -o my_prog_exec my_prog.c -L. lib/ADTlib.a
```

# Tests
The library comes with a somewhat comprehensive suite of tests covering the ADT's, using the [acutest](https://github.com/mity/acutest) library. The tests are located in the [tests](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/tests) directory, and they are designed to ensure that the library functions as expected.

---
© Pavlos Dais