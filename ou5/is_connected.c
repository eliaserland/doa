#include <stdio.h>
#include <string.h>	
#include <stdbool.h>	// Bool
#include <errno.h>	// Better error handeling
#include <ctype.h>	// isspace()

//#include "graph.h"


/*bool find_path(graph *g,node *src,node *dest)
{}*/





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







#define BUFSIZE 300             /* Max 300 char per line */

int main(int argc, char **argv)
{
	FILE *in; 			// Pointer to input file
	const char *filename;		// Input filename	
	char line[BUFSIZE];     	// Input buffer

	//int pos = 0; 		// Counter for file position
	

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
	bool first_line = true;
	int nr_edges;
	char node[40];
	char neighbour[40];

	while (fgets(line, BUFSIZE, in) != NULL) {
		if (line_is_blank(line) || line_is_comment(line)) {
                        // Ignore blank lines and comment lines.
                        continue;
                }
		
		if (first_line){
			sscanf(line,"%d", &nr_edges);
			first_line = false;
		} else {
			sscanf(line, "%40s %40s", node, neighbour);
			
			/* Insert nodes and edges intro graph.*/
		}
	}

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
