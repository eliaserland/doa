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
 * file detailing the structure of an arbitrary directed graph and answers 
 * the question whether or not there is a path between two nodes supplied by the
 * user.
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
		fprintf(stderr, "Could not open input file %s: %s\n", 
			filename, strerror(errno));
		exit(EXIT_FAILURE);
	} 

	// and input string buffer.
	char line[BUFSIZE];     

	// Pre-allocations
	bool first_line = true;
	int nr_edges;
	char node_src_str[40];
	char node_dst_str[40];
	graph *g; 

	// Read line from input file and repeat for each line until end-of-file.
	while (fgets(line, BUFSIZE, in)!= NULL) {
		// Ignore blank lines and comment lines.
		if (line_is_blank(line) || line_is_comment(line)) {
                        continue;
                }

		/* Extract the integer from the first line, create a new graph, 
		   then insert each following node pair. */
		if (first_line){
			sscanf(line,"%d", &nr_edges);
			//printf("Number of edges in graph: %d.\n", nr_edges);
			g = graph_empty(2*nr_edges);
			
			first_line = false;
		} else {
			sscanf(line, "%40s %40s", node_src_str, node_dst_str);
			//printf("Entry %d: %s --> %s.\n", tmp, node_str, neighbour);
			
			/* Insert the nodes intro the graph.*/
			graph_insert_node(g, node_src_str);
			graph_insert_node(g, node_dst_str);
			
			// Get the posititons of the recently inserted nodes. 
			node *node_src = graph_find_node(g, node_src_str);
			node *node_dst = graph_find_node(g, node_dst_str);

			// Insert the edge into the graph.
			graph_insert_edge(g, node_src, node_dst);
		}
	}

	// Close the input file.
	if (fclose(in)) {
                fprintf(stderr, "Failed to close input file %s: %s\n", filename,
			strerror(errno));
                exit(EXIT_FAILURE);
        }
	return g;
}

/** find_path() - Answers the question if there is a path from the source node 
 * 		  to the destination node.
 * @g: Pointer to the graph to be traversed.
 * @src: Pointer to the source node.
 * @dest: Pointer to the destination node.
 * 
 * Returns: True if there is a path from the source to the destination.
 */
bool find_path(graph *g,node *src,node *dest)
{	
	// Check if source and destination nodes are the same.
	if (nodes_are_equal(src, dest)) {
		return true;
	}

	/* Create a new and empty queue. Note: Freeing function must be set to 
	   NULL in order to not mistakingly remove any node from the graph on
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
	bool path_exists = false;	// Flag

	while (!queue_is_empty(q)) {
		// Pick first node from queue.
		n = queue_front(q);
		q = queue_dequeue(q);

		// Fetch list of neigbours.
		neighbour_list = graph_neighbours(g, n);

		// For each neighbour in list, ...
		dlist_pos pos = dlist_first(neighbour_list);
		while (!dlist_is_end(neighbour_list, pos)) {
			/* Inspect string describing neighbouring node and fetch 
			   the corresponding node pointer. */
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
	// Verify nr of input arguments.
	if (argc <= 1) {
		fprintf(stderr, "Useage: filename\n\twhere filename is a file " 
				"containing a directed graph structure.\n");
		return -1;
	} 
	
	// Create and populate graph according to file.
	const char *filename = argv[1];
	graph *g = populate_graph(filename);
	
	// run_program(graph *g)

	char str[255];
	char src[40];
	char dst[40];
	bool running = true;
	bool path_test;

	while (running) {
		printf("Enter origin and destination (quit to exit): ");
		fgets(str, 255, stdin);
		if (strncmp(str, "quit", 4) == 0) {
			running = false;
		} else {
			sscanf(str, "%40s %40s", src, dst);
			//printf("Source: %s, Destination: %s.\n", src, dst);

			// NEED TO CHECK USER INPUT !!!

			node *node_src = graph_find_node(g, src);
			node *node_dst = graph_find_node(g, dst);

			if (node_src == NULL) {
				printf("Node %s not found. Please input an existing node.\n", src);
				continue;
			} 
			if (node_dst == NULL) {
				printf("Node %s not found. Please input an existing node.\n", dst);
				continue;
			}
			
			path_test = find_path(g, node_src, node_dst);
			if (path_test) {
				printf("There is a path from %s to %s.\n", src, dst);
			} else {
				printf("There is no path from %s to %s.\n", src, dst);
			}
			
		}
	}

	graph_kill(g);
	printf("Normal exit.\n");
	return 0;
}
