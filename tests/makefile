# tested ADT 
# Vector/ Stack/ Queue/ PriorityQueue/ RedBlackTree/ HashTable/ DirectedGraph/ UndirectedGraph/ WeightedUndirectedGraph
ADT ?= HashTable

# compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

# ADTlib directory path
LIB = ../lib

# executable program
EXEC = test

# object file
OBJS = test_$(ADT).o

$(ADT): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) -L. $(LIB)/ADTlib.a

.PHONY: run help clear

# run the test
run: $(ADT)
	./$(EXEC)

# run valgrind - check for memory errors
help: $(EXEC)
	valgrind valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(EXEC)

# delete all files created during the test of the ADT
clear:
	rm -f $(OBJS) $(EXEC)
