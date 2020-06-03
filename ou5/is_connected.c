#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <stdbool.h>	
#include <errno.h>	
#include <ctype.h>	

#include "graph.h"
#include "queue.h"

#define BUFSIZE 300	/* Max 300 chars per line in the input map file. */

/* 
 * OU5 - Mandatory exercise 5 for the "Datastructures and algorithms" course at
 * the Department of Computing Science, Umea University. The program reads a map
 * file detailing the structure of an directed graph and interactively answers 
 * the question of whether or not there is a path from a source node to a 
 * destination node supplied by the user.
 * 
 * Author: Elias Olofsson (tfy17eon@cs.umu.se)
 * 
 * Based on earlier code by: Niclas Borlin (niclas.borlin@cs.umu.se)
 *
 * Version information:
 *   v1.0  2020-06-05: First public version.
*/

// ==========================================================================

/** 
 * first_non_white_space() - Find the first non-whitespace char of a string.
 * @s: Null-terminated string to be inspected.
 * 
 * Returns: The position of the first non-whitespace character, or -1 if only 
 * 	    whitespace is found.
 */
int first_non_white_space(const char *s)
{	
	/* Start at the first char. Advance until we hit EOL (NULL) as long as
	   we are loooking at white-space. */
        int i = 0; 
        while (s[i] && isspace(s[i])) {
                i++;
        }
        if (s[i]) {
                return i;  // Return position of the non-whitespace char.
        } else {
                return -1; // Return fail, only whitespace found.
        }
}

/**
 * line_is_blank() - Check if line is blank, i.e. only containing whitespace.
 * @s: Null-terminated string to be inspected.
 * 
 * Returns: True if s only contains whitespace characters.
 */
bool line_is_blank(const char *s)
{
        return first_non_white_space(s) < 0;
}

/**
 * line_is_comment() - Check if line is comment line.  
 * @s: Null-terminated string to be inspected.
 *
 * Returns: True if line is comment line, i.e. the first non-whitespace char is 
 * 	    '#'. 
 */
bool line_is_comment(const char *s)
{
        int i = first_non_white_space(s);
        return (i >= 0 && s[i] == '#');
}

/**
 * populate_graph() - Create and populate a new graph according to file.
 * @filename: A string containing the file name of the map file to open.
 *
 * Returns: Pointer to a new and populated graph.
 */
graph *populate_graph(const char *filename)
{
	// Allocate a file pointer and open the input file.
	FILE *in = fopen(filename, "r");
	if (in == NULL) {
		fprintf(stderr, "FAIL: Could not open the input file %s: %s\n", 
			filename, strerror(errno));
		exit(EXIT_FAILURE);
	} 

	/* Allocation of graph pointer, int for edge count, string buffers and 
	   flag for first line in file.	*/ 
	graph *g;
	int nr_edges;
	char line[BUFSIZE];
	char src_str[40];
	char dst_str[40];
	bool is_first_line = true;	 

	/* Read line from input file and repeat until end-of-file is reached.
	   Extract the integer from the first non-blank and non-comment line, 
	   signifying the number of edges to insert. Create a graph with 
	   max_nodes set to 2 times this value, to cover for the extreme case. 
	   For each following line, insert each pair of nodes and the 
	   corresponding edges into the graph. */
	while (fgets(line, BUFSIZE, in)!= NULL) {
		// Ignore blank lines and comment lines.
		if (line_is_blank(line) || line_is_comment(line)) {
                        continue;
                }

		// Single out the first non-comment, non-blank line.
		if (is_first_line){
			// Extract integer.
			int out = sscanf(line,"%d", &nr_edges);
			
			// Verify successful read by sscanf.
			if (out == EOF || out == 0) {
				fprintf(stderr, "FAIL: Incorrect map format. "
				        "Expected integer on first non-comment,"
					" non-blank line.\n");
				exit(EXIT_FAILURE);
			}

			// Create graph.
			g = graph_empty(2*nr_edges);
			
			// Flag that we have passed the first line of interest.
			is_first_line = false;
		} else {
			// Set first char to NULL in both string buffers. 
			src_str[0] = 0;
			dst_str[0] = 0;

			/* Extract the pair of nodes from the current line. Each
			   node label is a string of max 40 chars, separated by 
			   whitespace.	*/
			int out = sscanf(line, "%40s %40s", src_str, dst_str);
			
			// Verify successful read by sscanf.
			if (out == EOF) {
				fprintf(stderr, "FAIL: Could not interpret map "
				       "contents: %s.\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			
			// Verify that both strings have been written to.
			if (src_str[0] == 0 || dst_str[0] == 0) {
				fprintf(stderr, "FAIL: Incorrect map format. "
				        "Expected two alphanumeric node names "
					"on each line.\n");
				exit(EXIT_FAILURE);
			}

			// Insert the nodes into the graph.
			graph_insert_node(g, src_str);
			graph_insert_node(g, dst_str);
			
			// Get the posititons of the recently inserted nodes. 
			node *src = graph_find_node(g, src_str);
			node *dst = graph_find_node(g, dst_str);

			// Insert the edge into the graph.
			graph_insert_edge(g, src, dst);
		}
	}

	// Close the input file.
	if (fclose(in)) {
                fprintf(stderr, "FAIL: Could not close the input file %s: %s\n",
		        filename, strerror(errno));
                exit(EXIT_FAILURE);
        }
	return g;
}

/** find_path() - Answers the question whether or not there is a path from the 
 * 		  source node to the destination node.
 * @g: Pointer to the graph to be traversed.
 * @src: Pointer to the source node.
 * @dest: Pointer to the destination node.
 * 
 * Returns: True if there is a path from the source to the destination node.
 */
bool find_path(graph *g, node *src, node *dest)
{	
	// Check if source and destination nodes are the same.
	if (nodes_are_equal(src, dest)) {
		// Source and destination are the same node, always a path.
		return true;
	}

	/* Create a new and empty queue. Note: The freeing function must be set 
	   to NULL in order to not mistakingly remove any node from the graph on
	   dequeing. */
	queue *q = queue_empty(NULL);
	// Mark node as seen.
	graph_node_set_seen(g, src, true);
	// Place node in queue.
	q = queue_enqueue(q, src);

	// Allocate node and dlist pointers. 
	node *n;
	node *n_current;
	dlist *neighbour_list;
	// Flag to indicate that a path has been found.
	bool path_exists = false;

	/* Traverse the graph breadth-first from the source node. If the 
	   destination node is found, the two nodes are connected and an answer
	   to the question is found. */
	while (!queue_is_empty(q)) {
		// Pick and dequeue the first node in the queue.
		n = queue_front(q);
		q = queue_dequeue(q);

		// Fetch a list of the neighbours.
		neighbour_list = graph_neighbours(g, n);

		/* Inspect each neighbour in the list, mark as seen and add to 
		   the queue if not already visited. */
		dlist_pos pos = dlist_first(neighbour_list);
		while (!dlist_is_end(neighbour_list, pos)) {
			/* Inspect the string label and fetch the corresponding 
			   node pointer. */
			const char *s = dlist_inspect(neighbour_list, pos);
			n_current = graph_find_node(g, s);

			// Check if node has been seen before.
			if (!graph_node_is_seen(g, n_current)){
				/* Node has not already been seen. Mark as seen
				   and place in the queue. */
				graph_node_set_seen(g, n_current, true);
				q = queue_enqueue(q, n_current);
				
				// Check if current node is the destination node.
				if (nodes_are_equal(n_current, dest)) {
					// We have reached the destination node.
					path_exists = true;
					break;
				}
			}
			// Traverse the list to the next neighbour.
			pos = dlist_next(neighbour_list, pos);
		}
		// Destroy the list.
		dlist_kill(neighbour_list);

		// If the question has been answered, exit the while-loop.
		if (path_exists) {
			break;
		}
	}
	// Destroy the queue. 
	queue_kill(q);

	// Reset the seen status of all nodes in the graph.
	graph_reset_seen(g);

	// Return the answer to the question.
	return path_exists; 
}


int main(int argc, char **argv)
{
	// Verify the number of input arguments.
	if (argc <= 1) {
		fprintf(stderr, "Useage: filename\n\twhere filename is a map "
				"file containing a directed graph structure.\n");
		return -1;
	} 
	
	// Create and populate graph according to file.
	const char *filename = argv[1];
	graph *g = populate_graph(filename);
	
	// Allocate string buffers and test flags.
	char str[256];
	char src[40];
	char dst[40];
	bool running = true;
	bool path_test;
	
	/* While the user has not chosen to exit the program, ask for two nodes
	   as user input and answer whether or not there is a path from the 
	   source node to the destination node. */
	while (running) {
		// Print statement and get user input.
		printf("Enter origin and destination (quit to exit): ");
		fgets(str, 256, stdin);

		if (strncmp(str, "quit", 4) == 0) {
			// User has chosen to quit the program.
			running = false;
		} else {
			// Set first char to NULL in both string buffers.
			src[0] = 0;
			dst[0] = 0;

			// Interpret the input and save to each string buffer.
			int out = sscanf(str, "%40s %40s", src, dst);
			if (out == EOF) {
				printf("Could not interpret the input, please " 
					"try again.\n\n");
				continue;
			}

			// Verify that user has put in two separate arguments.
			if (src[0] == 0 || dst[0] == 0) {
				printf("Please enter two nodes, separated by " 
				       "whitespace.\n\n");
				continue;
			}
			
			// Find the corresponding node pointers in the graph. 
			node *node_src = graph_find_node(g, src);
			node *node_dst = graph_find_node(g, dst);

			// Check that both pointers are valid.
			if (node_src == NULL) {
				printf("Node %s not found. Please input an " 
				       "existing node.\n\n", src);
				continue;
			} 
			if (node_dst == NULL) {
				printf("Node %s not found. Please input an "
				       "existing node.\n\n", dst);
				continue;
			}
			
			/* Perform connectivity test and print the result. */
			path_test = find_path(g, node_src, node_dst);
			if (path_test) {
				printf("There is a path from %s to %s.\n\n", 
					src, dst);
			} else {
				printf("There is no path from %s to %s.\n\n", 
					src, dst);
			}	
		}
	}

	// Destroy the graph. 
	graph_kill(g);
	printf("Normal exit.\n");
	return 0;
}
