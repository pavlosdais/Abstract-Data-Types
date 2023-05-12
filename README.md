# Abstract Data Types in C
This is a modern C library containing implementations designed to provide fast and efficient data structure operations for a wide range of applications while also being educational, having a relatively non-challenging code to follow. All ADTs have been implemented using [void pointers](https://www.geeksforgeeks.org/void-pointer-c-cpp/) to make them generic, allowing the same code to handle different data types (eg integers, strings, structs, etc). The library contains the following ADTs:

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
Depending on the ADT, provide a number of the following functions upon its creation:
- **[Destroy function](https://github.com/pavlosdais/Abstract-Data-Types/blob/main/tests/test_HashTable.c#L9)** <br/>
    Destroys the data allocated for the elements.

- **[Compare function](https://github.com/pavlosdais/Abstract-Data-Types/blob/main/tests/include/common.h#L69)** <br/>
    Compares two Pointers a and b, and returns < 0 if a < b, 0 if a = b or > 0 if a > b.

- **[Hash function](https://github.com/pavlosdais/Abstract-Data-Types/blob/main/modules/HashTable/hash_functions.c)** <br/>
    Takes a Pointer and returns a unsigned integer (Used by the hash table).

- **[Visit function](https://github.com/pavlosdais/Abstract-Data-Types/blob/main/tests/test_DirectedGraph.c#L44)** <br/>
    Visits a vertex (Used by the graph).

### Step 3
Use `ADTlib.a` on compilation.
```bash
~$ gcc -o my_prog_exec my_prog.c -L. lib/ADTlib.a
```

# Tests
The library comes with a comprehensive suite of tests covering the ADT's, using the [acutest](https://github.com/mity/acutest) library. The tests are located in the [tests](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/tests) directory, and are designed to ensure that the library functions as expected. In order to test a certain ADT, navigate to the directory and simply execute:
```bash
~$ make run ADT=<ADT>
```

---
© Pavlos Dais