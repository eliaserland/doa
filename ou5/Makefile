# Makefile for OU5, DoA LP4 2020.
# 2020-05-20
#
# Useage:
# make 					- compile all binaries
# make clean				- clean all binaries 
# make memtest args=maps/1-airmap1.map	- test for memory leaks with valgrind

EXE = is_connected

SRC_queue = ../datastructures-v1.0.8.2/src/queue/queue.c
SRC_dlist = ../datastructures-v1.0.8.2/src/dlist/dlist.c
SRC_array_1d = ../datastructures-v1.0.8.2/src/array_1d/array_1d.c
SRC_list = ../datastructures-v1.0.8.2/src/list/list.c

SRC = $(SRC_queue) $(SRC_dlist) $(SRC_array_1d) $(SRC_list)

OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -std=c99 -Wall -g -I../datastructures-v1.0.8.2/include

all:	$(EXE)

# Object file for library
obj:	$(OBJ)

# Clean up
clean:
	-rm -f $(EXE) $(OBJ)
	
is_connected: is_connected.c graph.c $(SRC)
	gcc $^ -o $@ $(CFLAGS) 

memtest: is_connected
	valgrind --leak-check=full --show-reachable=yes ./$< $(args)