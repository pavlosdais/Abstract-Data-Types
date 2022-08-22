# ADT to be tested (Stack/ Queue/ PriorityQueue/ RedBlackTree/ HashTable)
ADT_TO_BE_TESTED = HashTable

# compiler
CC = gcc

# flags
CFLAGS = -Wall -Wextra -Werror -ggdb3

# object file
OBJS = test_$(ADT_TO_BE_TESTED).o

# ADTlib path
LIB = ../lib

$(ADT_TO_BE_TESTED): $(OBJS)
	$(CC) -o $(ADT_TO_BE_TESTED) $(OBJS) -L. $(LIB)/ADTlib.a

# run test
run: $(ADT_TO_BE_TESTED)
	./$(ADT_TO_BE_TESTED)

# run valgrind
help: $(ADT_TO_BE_TESTED)
	valgrind --leak-check=full -v --show-leak-kinds=all --track-origins=yes ./$(ADT_TO_BE_TESTED)

# delete all files created during the test of the ADT
clear:
	rm -f $(OBJS) $(ADT_TO_BE_TESTED)
