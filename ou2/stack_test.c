#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/*
 * OU2 DOA LP4 2020 - Test program for stack implementation in stack.c.
 *	
 * Author: Elias Olofsson (tfy17eon@cs.umu.se)
 *
 * Version information:
 * 	2020-04-15: v1.0. First public version.
 */

/*
 * Axiom 1 test: Isempty(Empty()) == true
 * Verify that a newly created stack is empty. Exits and prints an error 
 * message if stack is not empty.
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
 * Axiom 2, test 1: !Isempty(Push(v,s)) == true
 * Verify that if a value is pushed to a new stack, the stack is not empty. 
 * Exits and prints an error message if the stack is empty.  
 */
void nonempty_stack_test1()
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
 * Axiom 2, test 2: !Isempty(Push(v,s)) == true
 * Verify that if a value is pushed to a stack containing multiple elements, the
 * stack is not empty. Exits and prints an error message if the stack is empty.  
 */
void nonempty_stack_test2()
{
	stack *s = stack_empty(NULL);
	for (int i = 1; i <= 5; i++) {
		int *v = malloc(sizeof(*v));
		*v = i * i * i;
		s = stack_push(s,v);
	}

	if (stack_is_empty(s)) {
		fprintf(stderr, "FAIL: Stack with multiple element is empty!\n");
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
 * Axiom 4, test 1: Top(Push(v,s)) == v
 * Verify that a value pushed to an empty stack can be inspected to be at 
 * the top of that stack. Exits and prints an error message if the value at the
 * top of the stack is different from the one expected. 
 */
void one_element_test() 
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

/*
 * Axiom 4, test 2: Top(Push(v,s)) == v
 * Verify that a the value last pushed to a stack can be inspected to be at the
 * top of that stack. Exits and prints an error message if the value at the
 * top is different from the one expected. 
 */
void multiple_elements_test()
{
	stack *s = stack_empty(NULL);
	int last_pushed_value;
	for (int i = 1; i <= 5; i++) {
		int *v = malloc(sizeof(*v));
		*v = i * i;
		s = stack_push(s,v);
		last_pushed_value = *v;
	}
	
	int *value_at_top = stack_top(s);
	if (*value_at_top != last_pushed_value) {
		fprintf(stderr, "FAIL: Expected %d at top of stack, got %d!\n", 
			last_pushed_value, *value_at_top);
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
 * Axiom 3, test 1: Pop(Push(v,s)) == s
 * Verify that after pushing a value to a new stack, followed by removing
 * the top element of that same stack, we recover the the initial (empty) stack. 
 * Exits and prints an error message if the stack is not empty.
 */
void empty_push_pop_test()
{
	stack *s = stack_empty(NULL);
	int *v1 = malloc(sizeof(*v1));
	*v1 = 17;		
	s = stack_push(s,v1);
	
	int *v2 = stack_top(s);
	s = stack_pop(s);
	free(v2);
	
	if (!stack_is_empty(s)) {
		fprintf(stderr, "FAIL: Stack is not empty after push-pop!\n");
		exit(EXIT_FAILURE);
	}
	stack_kill(s);
}

/*
 * Axiom 3, test 2: Pop(Push(v,s)) == s
 * Verify that after pushing a value to a nonempty stack, followed by 
 * removing the top element from the same stack, we recover the inital stack. 
 * Exits and prints an error message if the stack has changed after the 
 * push-pop operations.
 */
void push_pop_test()
{
	// Create two identical stacks
	stack *s1 = stack_empty(NULL);
	stack *s2 = stack_empty(NULL);
	for (int i = 1; i <= 5; i++) {
		int *v1 = malloc(sizeof(*v1));
		int *v2 = malloc(sizeof(*v2));
		*v1 = i * 5;	
		*v2 = i * 5;		
		s1 = stack_push(s1,v1);
		s2 = stack_push(s2,v2);
	}
			
	// Push one value to s1
	int *v = malloc(sizeof(*v));
	*v = 100;
	s1 = stack_push(s1,v);
	
	// Pop one value from s1
	s1 = stack_pop(s1);
	free(v);
	
	// Compare stacks s1 and s2 while deallocating 
	while (!stack_is_empty(s1) || !stack_is_empty(s2)) {
		if (stack_is_empty(s1) != stack_is_empty(s2)) {
			fprintf(stderr, "FAIL: Stack has changed after push-pop!\n");
			exit(EXIT_FAILURE);					
		}
		int *v1 = stack_top(s1);
		int *v2 = stack_top(s2);
		if (*v1 != *v2) {
			fprintf(stderr, "FAIL: Stack has changed after push-pop!\n");
			exit(EXIT_FAILURE);
		}
		s1 = stack_pop(s1);
		s2 = stack_pop(s2);
		free(v1);
		free(v2);
	}
	stack_kill(s1);	
	stack_kill(s2);	
}


/*
 * Axiom 5, test 1: !Isempty(s) -> Push(Top(s),Pop(s)) == s
 * Verify that for a stack with one element, we recover the initial stack after 
 * removing the first element and then putting it back. Exits and prints an 
 * error message if the single valued stack has changed after the pop-push 
 * operation. 
 */
void minimal_pop_push_test() 
{
	// Create two identical stacks 
	stack *s1 = stack_empty(NULL);
	stack *s2 = stack_empty(NULL);
	int *v1 = malloc(sizeof(*v1));
	int *v2 = malloc(sizeof(*v2));	
	*v1 = 69;
	*v2 = 69;
	s1 = stack_push(s1,v1);
	s2 = stack_push(s2,v2);
	
	// Retrieve top value and pop top element in s1
	int *v = stack_top(s1);
	s1 = stack_pop(s1);
	
	// Push same value back to s1 
	s1 = stack_push(s1,v);
	
	// Compare stacks and dellocate
	if (stack_is_empty(s1) != stack_is_empty(s2)) {
		fprintf(stderr, "FAIL: Stack has changed after pop-push!\n");	
		exit(EXIT_FAILURE);
	} else {
		int *v1 = stack_top(s1);
		int *v2 = stack_top(s2);
		if (*v1 != *v2) {
			fprintf(stderr, "FAIL: Stack has changed after pop-push!\n");	
			exit(EXIT_FAILURE);
		}
		s1 = stack_pop(s1);
		s2 = stack_pop(s2);
		free(v1);
		free(v2);
	}
	stack_kill(s1);	
	stack_kill(s2);	
}

/*
 * Axiom 5, test 2: !Isempty(s) -> Push(Top(s),Pop(s)) == s
 * Verify that for a general non-empty stack, we recover the initial stack after 
 * removing the first element and then putting it back. Exits and prints an 
 * error message if the stack has changed (ordering and values) after the 
 * pop-push operation. 
 */
void pop_push_test() 
{
	// Create two identical stacks
	stack *s1 = stack_empty(NULL);
	stack *s2 = stack_empty(NULL);
	for (int i = 1; i <= 5; i++) {
		int *v1 = malloc(sizeof(*v1));
		int *v2 = malloc(sizeof(*v2));
		*v1 = i * 5;	
		*v2 = i * 5;		
		s1 = stack_push(s1,v1);
		s2 = stack_push(s2,v2);
	}
	
	// Retrieve top value and pop top element in s1
	int *v = stack_top(s1);
	s1 = stack_pop(s1);
	
	// Push same value back to s1
	s1 = stack_push(s1,v);
		
	// Compare stacks s1 and s2 while deallocating 
	while (!stack_is_empty(s1) || !stack_is_empty(s2)) {
		if (stack_is_empty(s1) != stack_is_empty(s2)) {
			fprintf(stderr, "FAIL: Stack has changed after pop-push!\n");
			exit(EXIT_FAILURE);					
		}
		int *v1 = stack_top(s1);
		int *v2 = stack_top(s2);
		if (*v1 != *v2) {
			fprintf(stderr, "FAIL: Stack has changed after pop-push!\n");
			exit(EXIT_FAILURE);
		}
		s1 = stack_pop(s1);
		s2 = stack_pop(s2);
		free(v1);
		free(v2);
	}
	stack_kill(s1);	
	stack_kill(s2);	
}

int main(void) 
{	
	fprintf(stderr, "Running TEST1: empty_stack_test(): ");
	empty_stack_test();
	fprintf(stderr, "OK.\n");
	fprintf(stderr, "Running TEST2: nonempty_stack_test1(): ");
	nonempty_stack_test1();
	fprintf(stderr, "OK.\n");
	fprintf(stderr, "Running TEST3: nonempty_stack_test2(): ");
	nonempty_stack_test2();
	fprintf(stderr, "OK.\n");
	fprintf(stderr, "Running TEST4: one_element_test(): ");
	one_element_test();
	fprintf(stderr, "OK.\n");
	fprintf(stderr, "Running TEST5: multiple_elements_test(): ");
	multiple_elements_test();
	fprintf(stderr, "OK.\n");
	fprintf(stderr, "Running TEST6: empty_push_pop_test(): ");
	empty_push_pop_test();
	fprintf(stderr, "OK.\n");
	fprintf(stderr, "Running TEST7: push_pop_test(): ");
	push_pop_test();
	fprintf(stderr, "OK.\n");
	fprintf(stderr, "Running TEST8: minimal_pop_push_test(): ");	
	minimal_pop_push_test();
	fprintf(stderr, "OK.\n");
	fprintf(stderr, "Running TEST9: pop_push_test(): ");
	pop_push_test();
	fprintf(stderr, "OK.\n");

	fprintf(stderr, "SUCCESS: Implementation passed all tests. Normal exit.\n");
	return 0;
}

