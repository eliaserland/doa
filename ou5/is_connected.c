#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

//#include "graph.h"


/*bool find_path(graph *g,node *src,node *dest)
{}*/

int main(int argc, char **argv)
{
	FILE *in; 		// Pointer to input file
	//int pos = 0; 		// Counter for file position
	const char *filename;		

	// Verify input arguments
	if (argc <= 1) {
		fprintf(stderr, "Useage: filename\n\twhere filename is a file " 
				"containing a directed graph structure.\n");
		return -1;
	} 
	
	// Try to open the input file
	filename = argv[1];
	in = fopen(filename, "r");
	if (in == NULL) {
		fprintf(stderr, "Couldn't open input file %s: %s\n", 
			filename, strerror(errno));
		return -1;
	} 
	
	// Repeat until end-of-file




	// Close file
	if (fclose(in)) {
                fprintf(stderr, "Failed to close %s: %s\n", filename,
			strerror(errno));
                return -1;
        }

	// Create graph

	// Main loop While
		// Ask user for input 

		// Perform search 


	return 0;
}
