# Test makefile for lecture 1
# Useage:
# make		# compile all binaries
# make clean 	# remove all binaries and objects

EXE = dcount_abstract dcount_dynamic dcount_indexed

SRC = ../datastructures-v1.0.8.2/src/int_array_1d/int_array_1d.c
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -std=c99 -Wall -I../datastructures-v1.0.8.2/include -g


all: exe

# Executeable files
exe: $(EXE)

# Object file for library
obj: $(OBJ)

# Clean up
clean:
	rm -f $(EXE) $(OBJ)
	
%: %.c $(SRC)
	$(CC) -o $@ $(CFLAGS) $^ 
	
	
