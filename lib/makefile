# path to the modules directory
ADTs = ../modules

# implementation of the hash table (LinearHashing/ SeparateChaining/ DoubleHashing/ UsingRBT)
HT_IMPLEMENTATION = DoubleHashing

# object files
OBJ = $(ADTs)/Stack/stack.o \
	  $(ADTs)/Queue/queue.o \
	  $(ADTs)/PriorityQueue/pq.o \
	  $(ADTs)/RedBlackTree/RedBlackTree.o \
	  $(ADTs)/HashTable/$(HT_IMPLEMENTATION)/hash_table.o \
	  $(ADTs)/HashTable/hash_functions.o

# library
LIB = ADTlib.a

# compiler
CC = gcc

# create library
lib: $(OBJ)
	ar rcs $(LIB) $(OBJ)
	rm -f $(OBJ)

# delete library
clear:
	rm -f $(LIB)
