#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "graph.h"
#include "array_1d.h"
#include "dlist.h"


/*	OLD HEADER - CHANGE THIS TEXT
 * Declaration of a generic graph for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University. The graph stores nodes and edges of a directed or
 * undirected graph. After use, the function graph_kill() must
 * be called to de-allocate the dynamic memory used by the graph
 * itself. The de-allocation of any dynamic memory allocated for the
 * node names is the responsibility of the user of the graph.
 *
 * Author: Niclas Borlin (niclas.borlin@cs.umu.se)
 *
 * Version information:
 *   v1.0  2019-02-21: First public version.

 *   v1.01 2019-02-26: Modified include directives for header files
 *                     from the codebase to use "" to handle case when
 *                     student stores all header files in the current
 *                     directory. See
 *                     https://stackoverflow.com/questions/21593/what-is-the-difference-between-include-filename-and-include-filename.
 *   v1.02 2019-03-01: Doc update in graph_node_is_seen().
 *   v1.1  2019-03-06: Changed several const node * to node *.
 *                     Fixed doc bug to state that any dynamic memory allocated
 *                     to the node NAMES is the resposibility of the user.
 */

// ====================== PUBLIC DATA TYPES ==========================

// Anonymous declarations of node and graph.
struct node {
	char *label;
	bool is_seen;
	dlist *neighbours;
}; 

struct graph {
	array_1d *node_array;
};

// ===================== INTERNAL FUNCTIONS ==========================

/**
 * copy_string() - Create a dynamic copy of a string.
 * @s: String to be copied.
 *
 * Allocates memory for a dynamic copy of s and copies the contents of
 * s into the copy.
 *
 * Returns: Pointer to the copy of s.
 */
char *copy_string(const char *s)
{
        int len=strlen(s);

        /* Allocate memory for new string, with an extra char for \0 */
        char *dest = malloc(sizeof(char)*(len+1));

        /* Malloc failed, return NULL */
        if (dest == NULL) {
                return NULL;
        }

        /* Copy content to new memory */
        strncpy(dest, s, len);

        /* Strings should always be null terminated */
        dest[len] = '\0';
        return dest;
}

/**
 * clone_dlist() - Create a dynamic copy of a dlist.
 * @l: pointer to dlist to clone.
 * 
 * Allocates memory and clones the contents of l into a new dynamic directed 
 * list. List has to contain strings as values.  
 * 
 * Returns: Pointer to the copy of l. 
 */
dlist *clone_dlist(const dlist* l)
{
	// Create new and empty list.
	dlist *new_list = dlist_empty(free);

	// Get first position in each list.
	dlist_pos p_old = dlist_first(l);
	dlist_pos p_new = dlist_first(new_list);

	while (!dlist_is_end(l, p_old)) {
		// Fetch string from list
		const char *str = dlist_inspect(l, p_old);

		// Create dyamic copy of the string.
		const char *str_cpy = copy_string(str);
		
		// Insert string into the new list.
		p_new = dlist_insert(new_list, str_cpy, p_new);

		// Traverse the lists.
		p_old = dlist_next(l, p_old);
		p_new = dlist_next(new_list, p_new);
	}
	return new_list; 
}

// =================== NODE COMPARISON FUNCTION ======================

/**
 * nodes_are_equal() - Check whether two nodes are equal.
 * @n1: Pointer to node 1.
 * @n2: Pointer to node 2.
 *
 * Returns: true if the nodes are considered equal, otherwise false.
 */
bool nodes_are_equal(const node *n1, const node *n2)
{
	const char *s1 = n1->label;
	const char *s2 = n2->label;
	return strcmp(s1, s2) == 0;
}

// =================== GRAPH STRUCTURE INTERFACE ======================

/**
 * graph_empty() - Create an empty graph.
 * @max_nodes: The maximum number of nodes the graph can hold.
 *
 * Returns: A pointer to the new graph.
 */
graph *graph_empty(int max_nodes)
{
	// Allocate the graph header.
	graph *g = calloc(1, sizeof(graph));
	// Create array to hold the nodes. Size of array is [0, max_nodes-1].
	g->node_array = array_1d_create(0, max_nodes-1, NULL);

	return g;
}

/**
 * graph_is_empty() - Check if a graph is empty, i.e. has no nodes.
 * @g: Graph to check.
 *
 * Returns: True if graph is empty, otherwise false.
 */
bool graph_is_empty(const graph *g)
{

}

/**
 * graph_has_edges() - Check if a graph has any edges.
 * @g: Graph to check.
 *
 * Returns: True if graph has any edges, otherwise false.
 */
bool graph_has_edges(const graph *g) 
{

}

/**
 * graph_insert_node() - Inserts a node with the given name into the graph.
 * @g: Graph to manipulate.
 * @s: Node name.
 *
 * Creates a new node with a copy of the given name and puts it into
 * the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_node(graph *g, const char *s) // Need
{
	/* While current position in array is nonempty, check for duplicate 
	   node. */
	int i = 0;
	while(array_1d_has_value(g->node_array, i)) {
		// Inspect node
		node *n = array_1d_inspect_value(g->node_array, i);
		if (strcmp(s, n->label) == 0) {
			// Duplicate node, return the unaltered graph.
			return g;
		}
		i++;
	}
	/* Current position in the array is empty. Allocate and insert a new 
	   node structure at this position. */
	node *n = malloc(sizeof(node));

	// Make a dynamic copy of the input string and insert into the node.
	const char *str = copy_string(s);
	n->label = str;

	// Initialize the node as not seen.
	n->is_seen = false;

	// Create a directed list to hold the node's neighbours.
	dlist *l = dlist_empty(NULL);
	n->neighbours = l;

	// Insert the node structure into the array.
	array_1d_set_value(g->node_array, n, i);

	return g;
}

/**
 * graph_find_node() - Find a node stored in the graph.
 * @g: Graph to manipulate.
 * @s: Node identifier, e.g. a char *.
 *
 * Returns: A pointer to the found node, or NULL.
 */
node *graph_find_node(const graph *g, const char *s)
{
	// Traverse array until node is found, else return NULL.
	int i = 0;
	while (array_1d_has_value(g->node_array, i)) {
		node *n = array_1d_inspect_value(g->node_array, i);
		if (strcmp(s, n->label) == 0) {
			// Node match, return pointer to node. 
			return n;
		}
		i++; 
	}
	// No match found, return NULL.
	return NULL;
}

/**
 * graph_node_is_seen() - Return the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to return seen status for.
 *
 * Returns: The seen status for the node.
 */
bool graph_node_is_seen(const graph *g, const node *n)
{
	return n->is_seen;
}

/**
 * graph_node_set_seen() - Set the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to set seen status for.
 * @s: Status to set.
 *
 * Returns: The modified graph.
 */
graph *graph_node_set_seen(graph *g, node *n, bool seen) 
{
	n->is_seen = seen;
	return g;
}

/**
 * graph_reset_seen() - Reset the seen status on all nodes in the graph.
 * @g: Graph to modify.
 *
 * Returns: The modified graph.
 */
graph *graph_reset_seen(graph *g)
{
	int i = 0;
	while (array_1d_has_value(g->node_array, i)) {
		node *n = array_1d_inspect_value(g->node_array, i);
		n->is_seen = false;
		i++;
	}
	return g;
}

/**
 * graph_insert_edge() - Insert an edge into the graph.
 * @g: Graph to manipulate.
 * @n1: Source node (pointer) for the edge.
 * @n2: Destination node (pointer) for the edge.
 *
 * NOTE: Undefined unless both nodes are already in the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_edge(graph *g, node *n1, node *n2)
{
	int i = 0;
	bool n1_found = false;
	bool n2_found = false;
	dlist *neighbourlist;
	while (array_1d_has_value(g->node_array, i)) {
		node *n = array_1d_inspect_value(g->node_array, i);
		if (nodes_are_equal(n, n1)) {
			// Source node match, flag and save dlist pointer.
			n1_found = true;
			neighbourlist = n->neighbours;
		}
		if (nodes_are_equal(n, n2)) {
			// Destination node match, flag.
			n2_found = true;
		}
		i++; 
	}

	/* If both nodes are found, insert the edge. Otherwise return  the 
	   unaltered graph. */
	if (n1_found && n2_found) {
		/* Make a dynamic copy of the destination node label and insert 
		   into the list of neighbours for the source node.*/
		const char *str = copy_string(n2->label);
		dlist_pos pos =  dlist_first(neighbourlist);
		dlist_insert(neighbourlist, str, pos);
	}

	return g;
}

/**
 * graph_delete_node() - Remove a node from the graph.
 * @g: Graph to manipulate.
 * @n: Node to remove from the graph.
 *
 * Returns: The modified graph.
 *
 * NOTE: Undefined if the node is not in the graph.
 */
graph *graph_delete_node(graph *g, node *n) 
{

}

/**
 * graph_delete_edge() - Remove an edge from the graph.
 * @g: Graph to manipulate.
 * @n1: Source node (pointer) for the edge.
 * @n2: Destination node (pointer) for the edge.
 *
 * Returns: The modified graph.
 *
 * NOTE: Undefined if the edge is not in the graph.
 */
graph *graph_delete_edge(graph *g, node *n1, node *n2) 
{

}

/**
 * graph_choose_node() - Return an arbitrary node from the graph.
 * @g: Graph to inspect.
 *
 * Returns: A pointer to an arbitrayry node.
 *
 * NOTE: The return value is undefined for an empty graph.
 */
node *graph_choose_node(const graph *g)
{

}

/**
 * graph_neighbours() - Return a list of neighbour nodes.
 * @g: Graph to inspect.
 * @n: Node to get neighbours for.
 *
 * Returns: A pointer to a list of nodes. Note: The list must be
 * dlist_kill()-ed after use.
 */
dlist *graph_neighbours(const graph *g,const node *n)
{
	int i = 0;
	dlist *l;
	while (array_1d_has_value(g->node_array, i)) {
		node *n_current = array_1d_inspect_value(g->node_array, i);
		if (nodes_are_equal(n, n_current)) {
			/* Node match. Create a dlist copy of this node's 
			neighbours to return. */
			l = clone_dlist(n->neighbours);
		}
		i++;		
	}
	return l;
}

/**
 * graph_kill() - Destroy a given graph.
 * @g: Graph to destroy.
 *
 * Return all dynamic memory used by the graph.
 *
 * Returns: Nothing.
 */
void graph_kill(graph *g)
{
	// Traverse the array and deallocate each node structure. 
	int i = 0;
	while(array_1d_has_value(g->node_array, i)) {
		// Inspect the node.
		node *n = array_1d_inspect_value(g->node_array, i);
		
		// Traverse the list and deallocate each value.
		dlist_pos p = dlist_first(n->neighbours); 
		while (!dlist_is_end(n->neighbours, p)) {
			char *v = dlist_inspect(n->neighbours, p);
			free(v);
			p = dlist_remove(n->neighbours, p);
		}
		// Destroy the list.
		dlist_kill(n->neighbours);

		// Deallocate the label and the node strucure itself.
		free(n->label);
		free(n);
	}
	// Destroy the array and the graph itself.
	array_1d_kill(g->node_array);
	free(g);
}

/**
 * graph_print() - Iterate over the graph elements and print their values.
 * @g: Graph to inspect.
 *
 * Iterates over the graph and prints its contents.
 *
 * Returns: Nothing.
 */
void graph_print(const graph *g)
{

}

