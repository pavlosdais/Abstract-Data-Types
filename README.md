# Abstract Data Types in C
Implemented ADTs:
* Stack
* Queue
* Priority Queue
* Red-Black Tree
* Hash Table

The source code of every ADT can be found over at the `modules` directory.

# Usage
These ADTs are fully modular and designed to work with any kind of data. This is achieved by saving all the data (eg integers, strings, structs etc) in the heap by manually allocating memory and using [void pointers](https://www.geeksforgeeks.org/void-pointer-c-cpp/) to refer to it. In order to use a module one must provide a number of the following functions (depending on the ADT) upon initialization:
* `Destroy function` - destroys the data (or NULL if you do not want the data to be deleted).
* `Compare function` - compares two Pointers a and b, and returns -1 if a < b, 0 if a = b or 1 if a > b.
* `Hash function` - used by hash tables, takes a Pointer(our data) and returns a positive integer.

# Library containing all the ADTs
All ADTs implemented are included in the `ADTlib.a` library, over at directory `lib`. In order to use the library, include the library's header file `ADT.h` as well as use `ADTlib.a` on compilation. The library can be recompiled by running `make`. Check the `tests` directory for some examples on its usage.
