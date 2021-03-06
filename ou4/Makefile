# Makefile for OU4, Author: Elias Olofsson
# DOA LP4 2020
#
# Useage:
#   make 				   - Compile all test programs
#   make clean				   - Clean all binaries
#   make obj				   - Compile all object files 
#
#   make tabletest	   		   - Compile specific test program
#   make mtftabletest
#   make arraytabletest
#   make hashtabletest
#
#   make memtest args="-n -t 1000"	   - Test for memory leaks with valgrind
#   make memtest_table2 args="-n -t 1000"
#   make memtest_mtf args="-n -t 1000"
#   make memtest_array args="-n -t 1000" 
#   make memtest_hash args="-n -t 1000"

EXE = tabletest mtftabletest arraytabletest hashtabletest

SRC_table2 = ../datastructures-v1.0.8.2/src/table/table2.c
OBJ_table2 = $(SRC_table2:.c=.o)

SRC_dlist = ../datastructures-v1.0.8.2/src/dlist/dlist.c
OBJ_dlist = $(SRC_dlist:.c=.o)

SRC_array = ../datastructures-v1.0.8.2/src/array_1d/array_1d.c
OBJ_array = $(SRC_array:.c=.o)

SRC_hash = ../datastructures-v1.0.8.2/src/array_1d/array_1d.c
OBJ_hash = $(SRC_array:.c=.o)

OBJ = $(OBJ_table2) $(OBJ_dlist) $(OBJ_array) $(OBJ_hash)

CC = gcc
CFLAGS = -std=c99 -Wall -g -I../datastructures-v1.0.8.2/include


all: exe

# All executable test programs
exe: $(EXE)

# Object file for library
obj: $(OBJ)

memtest: memtest_table2 memtest_mtf memtest_array memtest_hash

# Clean up
clean:
	-rm -f $(EXE) $(OBJ)
	
tabletest: tabletest-1.9.c $(SRC_table2) $(SRC_dlist) 
	gcc -o $@ $(CFLAGS) $^

mtftabletest: tabletest-1.9.c mtftable.c $(SRC_dlist)
	gcc -o $@ $(CFLAGS) $^

arraytabletest: tabletest-1.9.c	arraytable.c $(SRC_array) 
	gcc -o $@ $(CFLAGS) $^
	
hashtabletest: tabletest-1.9.c	hashtable.c $(SRC_array)
	gcc -o $@ $(CFLAGS) $^

memtest_table2:	tabletest
	valgrind --leak-check=full --show-reachable=yes ./$< $(args)
	
memtest_mtf: mtftabletest
	valgrind --leak-check=full --show-reachable=yes ./$< $(args)
	
memtest_array: arraytabletest
	valgrind --leak-check=full --show-reachable=yes ./$< $(args)
	
memtest_hash: hashtabletest
	valgrind --leak-check=full --show-reachable=yes ./$< $(args)
	
