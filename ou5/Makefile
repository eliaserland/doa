# Makefile for OU5, DoA LP4 2020.
# 2020-05-20
#
# Useage:
# make 					- compile all binaries
# make clean				- clean all binaries 
# make memtest file=maps/1-airmap1.map	- test for memory leaks with valgrind

EXE = is_connected

#SRC = ../datastructures-v1.0.8.2/src/stack/stack.c
#OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -std=c99 -Wall -g	#-I../datastructures-v1.0.8.2/include

all:	$(EXE)

# Object file for library
obj:	$(OBJ)

# Clean up
clean:
	-rm -f $(EXE) $(OBJ)
	
is_connected: is_connected.c
	gcc $^ -o $@ $(CFLAGS) 

memtest: is_connected
	valgrind --leak-check=full --show-reachable=yes ./$< $(file)