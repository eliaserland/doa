# Makefile for OU2, compilation of stack_test.c w/ dependencies
# DOA LP4 2020

EXE = stack_test

SRC = ../datastructures-v1.0.8.2/src/stack/stack.c
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -std=c99 -Wall -I../datastructures-v1.0.8.2/include -g

all:	stack_test

# Object file for library
obj:	$(OBJ)

# Clean up
clean:
	-rm -f $(EXE) $(OBJ)
	
stack_test: stack_test.c $(SRC)
	gcc -o $@ $(CFLAGS) $^

memtest: stack_test
	valgrind --leak-check=full --show-reachable=yes ./$<

