# tested ADT 
# Vector/ Stack/ Queue/ PriorityQueue/ RedBlackTree/ HashTable/ DirectedGraph/ UndirectedGraph/ WeightedUndirectedGraph
ADT_TO_BE_TESTED = HashTable

# compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

# ADTlib directory path
LIB = ../lib

# executable program
EXEC = test

# object file
OBJS = test_$(ADT_TO_BE_TESTED).o

$(ADT_TO_BE_TESTED): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) -L. $(LIB)/ADTlib.a

# run the test
run: $(EXEC)
	./$(EXEC)

# run valgrind - check for memory errors
help: $(EXEC)
	valgrind --leak-check=full -v --show-leak-kinds=all --track-origins=yes ./$(EXEC)

# delete all files created during the test of the ADT
clear:
	rm -f $(OBJS) $(EXEC)
