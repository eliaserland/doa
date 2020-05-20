#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <stdbool.h>	// Bool
#include <errno.h>	// Better error handeling
#include <ctype.h>	// isspace()

//#include "graph.h"

#define BUFSIZE 300             /* Max 300 char per line */







/* Return position of first non-whitespace character or -1 if only
   white-space is found. */
int first_non_white_space(const char *s)
{
        int i = 0; // Start at first char.
        // Advance until we hit EOL as long as we're loooking at white-space.
        while (s[i] && isspace(s[i])) {
                i++;
        }
        if (s[i]) {
                return i; // Return position of found a non-white-space char.
        } else {
                return -1; // Return fail.
        }
}

/* Return true if s only contains whitespace */
bool line_is_blank(const char *s)
{
        // Line is blank if it only contained white-space chars.
        return first_non_white_space(s) < 0;
}

/* Return true if s is a comment line, i.e. first non-whitespc char is '#' */
bool line_is_comment(const char *s)
{
        int i = first_non_white_space(s);
        return (i >= 0 && s[i] == '#');
}

/**
 * populate_graph() - Create and populate a new graph according to file.
 * @filename: A string containing the file name of file to open.
 *
 * Returns: Pointer to a new and populated graph.
 */
void populate_graph(const char *filename) // CHANGE RETURN TYPE !!!!!!!!!!!!!!!
{
	FILE *in; 			// Pointer to input file
	char line[BUFSIZE];     	// Input buffer
	
	// Try to open the input file
	in = fopen(filename, "r");
	if (in == NULL) {
		fprintf(stderr, "Couldn't open input file %s: %s\n", 
			filename, strerror(errno));
		exit(EXIT_FAILURE);
	} 

	// Repeat until end-of-file
	bool first_line = true;
	int nr_edges;
	char node_str[40];
	char neighbour[40];

	int tmp = 0;
	while (fgets(line, BUFSIZE, in) != NULL) {
		if (line_is_blank(line) || line_is_comment(line)) {
                        // Ignore blank lines and comment lines.
                        continue;
                }

		/* Extract integer from first line, create a new graph, then 
		insert each following node pair. */
		if (first_line){
			sscanf(line,"%d", &nr_edges);
			printf("Number of edges in graph: %d.\n", nr_edges);
			//graph *g = graph_empty(nr_edges);
			
			first_line = false;
		} else {
			sscanf(line, "%40s %40s", node_str, neighbour);
			printf("Entry %d: %s --> %s.\n", tmp, node_str, neighbour);
			tmp++;
			/* Insert nodes and edges intro graph.*/
		}
	}

	// Close file
	if (fclose(in)) {
                fprintf(stderr, "Failed to close %s: %s\n", filename,
			strerror(errno));
                exit(EXIT_FAILURE);
        }
}

/*
bool find_path(graph *g,node *src,node *dest)
{

}
*/

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
	populate_graph(filename);
	//graph *g = populate_graph(filename);
	
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
			printf("Source: %s, Destination: %s.\n", src, dst);

			// NEED TO CHECK USER INPUT !!!

			path_test = find_path(g, src, dst);
			if (path_test) {
				printf();
			} else {
				printf();
			}
			

			/* Perform search */
		}
	}

	printf("Normal exit.\n");
	return 0;
}
