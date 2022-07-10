# Abstract Data Structures in C
Implemented data structures
* Stack
* Queue
* Priority Queue
* Red-Black Tree
* Hash Table (Seperate chaining & Linear hashing)

# Usage
These data structures are fully modular and designed to work with any kind of data. This is achieved by saving all the data in the heap by manually allocating memory and using [void pointers](https://www.geeksforgeeks.org/void-pointer-c-cpp/) to refer to it. In order to use a module one must provide the following following functions upon initialization:
* Destroy function - or NULL if you want want to delete the data.
* Compare function - compares two Pointer a and b and returns -1 if a < b, 0 if a = b or 1 if a > b.
* Hash function - used by hash tables, takes a Pointer(our data) and returns a positive integer.