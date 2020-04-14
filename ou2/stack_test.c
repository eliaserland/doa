#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

/*
 * OU2 DOA LP4 2020 - Test program for stack implementaion in stack.c.
 *	
 * Author: Elias Olofsson (tfy17eon@cs.umu.se)
 *
 * Version information:
 * 	2020-04-XX: v1.0. First public version.
 */


/*
 *	Axiom 1 test: Isempty(Empty()) == true
 * 	Verify that a newly created stack is empty. 
 */
void empty_stack_test()
{
	stack *s = stack_empty(NULL);
	if (!stack_is_empty(s)) {
		fprintf(stderr, "FAIL: New stack is not empty!\n");
		exit(EXIT_FAILURE);
	}	
	stack_kill(s);
}


/*
 *	Axiom 2 test: !Isempty(Push(v,s)) == true
 *	Verify that if an element is pushed to a new stack, the stack is no 
 *	longer empty.   
 */
void one_element_test()
{
	stack *s = stack_empty(NULL);
	int *v = malloc(sizeof(*v));
	*v = 19;	
	s = stack_push(s,v); 
	if (stack_is_empty(s)) {
		fprintf(stderr, "FAIL: Stack with one element is empty!\n");
		exit(EXIT_FAILURE);
	}
	
	while (!stack_is_empty(s)) {
		int *v = stack_top(s);
		s = stack_pop(s);
		free(v);
	}
	stack_kill(s);
}

/*
 *	Axiom 4 test: Top(Push(v,s)) == v
 *	Verify that an element pushed to the top of a stack can be retrived 
 * 	from the top of the stack. 	
 */
void element_value_test() 
{
	stack *s = stack_empty(NULL);
	int *v = malloc(sizeof(*v));
	*v = 21;		
	s = stack_push(s,v);
	
	int *value_at_top = stack_top(s);
	if (*value_at_top != 21) {
		fprintf(stderr, "FAIL: Expected %d at top of stack, got %d!\n", 
			21, *value_at_top);
		exit(EXIT_FAILURE);
	}
	
	while (!stack_is_empty(s)) {
		int *v = stack_top(s);
		s = stack_pop(s);
		free(v);
	}
	stack_kill(s);	
}



int main(void) 
{	
	fprintf(stderr, "Running TEST1: empty_stack_test(): ");
	empty_stack_test();
	fprintf(stderr, "OK.\n");
	
	fprintf(stderr, "Running TEST2: one_element_test(): ");
	one_element_test();
	fprintf(stderr, "OK.\n");
	
	fprintf(stderr, "Running TEST3: element_value_test(): ");
	element_value_test();
	fprintf(stderr, "OK.\n");


	fprintf(stderr, "SUCCESS: Implementation passed all tests. Normal exit.\n");

	return 0;
}

