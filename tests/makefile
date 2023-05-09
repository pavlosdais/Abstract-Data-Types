# ADT to be tested (Vector/ Stack/ Queue/ PriorityQueue/ RedBlackTree/ HashTable/ DirectedGraph/ UndirectedGraph/ WeightedUndirectedGraph)
ADT_TO_BE_TESTED = HashTable

CFLAGS = -Wall -Wextra -Werror -g

# compiler
CC = gcc

# object file
OBJS = test_$(ADT_TO_BE_TESTED).o

# ADTlib path
LIB = ../lib

$(ADT_TO_BE_TESTED): $(OBJS)
	$(CC) $(CFLAGS) -o $(ADT_TO_BE_TESTED) $(OBJS) -L. $(LIB)/ADTlib.a

# run test
run: $(ADT_TO_BE_TESTED)
	./$(ADT_TO_BE_TESTED)

# run valgrind - check for memory errors
help: $(ADT_TO_BE_TESTED)
	valgrind --leak-check=full -v --show-leak-kinds=all --track-origins=yes ./$(ADT_TO_BE_TESTED)

# delete all files created during the test of the ADT
clear:
	rm -f $(OBJS) $(ADT_TO_BE_TESTED)
