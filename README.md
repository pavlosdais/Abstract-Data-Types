# Abstract Data Types in C
Implemented ADTs:
* [Stack](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules/Stack#readme)
* [Queue](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules/Queue#readme)
* [Priority Queue](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules/PriorityQueue#readme)
* [Red-Black Tree](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules/RedBlackTree#readme)
* [Hash Table](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules/HashTable#readme)
* [Graph](https://github.com/pavlosdais/Abstract-Data-Types/tree/main/modules/Graph#readme)

The source code of every ADT can be found over at the `modules` directory.

# Usage
These ADTs are fully modular and designed to work with any kind of data. This is achieved by saving all the data (eg integers, strings, structs etc) in the heap by manually allocating memory and using [void pointers](https://www.geeksforgeeks.org/void-pointer-c-cpp/) to refer to it. In order to use a module one must provide a number of the following functions (depending on the ADT) upon initialization:
* `Destroy function` - destroys the data (or NULL if you do not want the data to be deleted).
* `Compare function` - compares two Pointers a and b, and returns -1 if a < b, 0 if a = b or 1 if a > b.
* `Hash function` - used by the hash table, takes a Pointer(our data) and returns a positive integer.
* `Visit function` - used by the graph, visits a vertex.

# Library containing all the ADTs
All ADTs implemented are included in the `ADTlib.a` library, over at directory `lib`. In order to use the library:
* Include the library's header file `ADT.h` *or* just the header of the module you want to use.
* Use `ADTlib.a` on compilation.

The library can be recompiled by running `make`. Check the `tests` directory for some examples on its usage.
