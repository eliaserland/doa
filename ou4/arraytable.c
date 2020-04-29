#include <stdlib.h>
#include <stdio.h>

#include "table.h"
#include "array_1d.h"

/*
 * Implementation of a generic table for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University. Table constructed as an one dimensional array.
 *
 * Duplicates are handled at element insertion.
 *
 * Author: Elias Olofsson (tfy17eon@cs.umu.se) 
 *
 * Based on earlier code by: Niclas Borlin (niclas@cs.umu.se),
 *	   	 	     Adam Dahlgren Lindstrom (dali@cs.umu.se),
 * 			     Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   2020-05-XX: v1.0, first public version.   
 */

// ==================INTERNAL CONSTANTS=====================

// Size of table to generate
#define TABLE_SIZE 80000

// ==================INTERNAL DATA TYPES====================

struct table {
	array_1d *entries;
	compare_function *key_cmp_func;
	free_function key_free_func;
	free_function value_free_func;
	int index_last_pos;
};

struct table_entry {
	void *key;
	void *value;
};

// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
 * table_empty() - Create an empty table.
 * @key_cmp_func: A pointer to a function to be used to compare keys.
 * @key_free_func: A pointer to a function (or NULL) to be called to
 *		   de-allocate memory for keys on remove/kill.
 * @value_free_func: A pointer to a function (or NULL) to be called to
 *		     de-allocate memory for values on remove/kill.
 *
 * Returns: Pointer to a new table.
 */
table *table_empty(compare_function *key_cmp_func,
		   free_function key_free_func,
		   free_function value_free_func)
{
	// Allocate the table header.
	table *t = calloc(1, sizeof(table));
	// Create the array to hold the table_entry-ies. Size of table will be 
	// [0, TABLE_SIZE-1].
	t->entries = array_1d_create(0 ,TABLE_SIZE-1 , NULL);
	// Store the key compare function and key/value free functions.
	t->key_cmp_func = key_cmp_func;
	t->key_free_func = key_free_func;
	t->value_free_func = value_free_func;
	t->index_last_pos = -1;

	return t;
}

/**
 * table_is_empty() - Check if a table is empty.
 * @table: Table to check.
 *
 * Returns: True if table contains no key/value pairs, false otherwise.
 */
bool table_is_empty(const table *t)
{
	return t->index_last_pos == -1;
}

/**
 * table_insert() - Add a key/value pair to a table.
 * @table: Table to manipulate.
 * @key: A pointer to the key value.
 * @value: A pointer to the value value.
 *
 * Insert the key/value pair into the table. Starts from the beginning of the 
 * array and checks for key duplicates until either a matching key or an empty 
 * element is discovered.
 *
 * Returns: Nothing.
 */
void table_insert(table *t, void *key, void *value)
{
	// While current position is nonempty, check for duplicate key.
	int i = 0;
	while (array_1d_has_value(t->entries, i)) {
		// Inspect table entry
		struct table_entry *entry = array_1d_inspect_value(t->entries, i);
		
		// Check if keys match in current table_entry
		if ((t->key_cmp_func(entry->key, key)) == 0) {
			// If we have a key match, call free function for the 
			// old key/value pair of this entry. 
			if (t->key_free_func != NULL) {
				t->key_free_func(entry->key);		
			}
			if (t->value_free_func != NULL) {
				t->value_free_func(entry->value);	
			}
			// Set pointers to new key/value pair and exit.
			entry->key = key;
			entry->value = value;
			return;			
		}
		i++;
	}
	// Current position in table is empty, allocate and insert new key/value 
	// structure at this position.
	struct table_entry *entry = malloc(sizeof(struct table_entry));	
	entry->key = key;
	entry->value = value;
	
	// Set pointer to table entry in array, increment pos of last element 
	// in array.
	array_1d_set_value(t->entries, entry, i);
	t->index_last_pos++;	
}

/**
 * table_lookup() - Look up a given key in a table.
 * @table: Table to inspect.
 * @key: Key to look up.
 *
 * Returns: The value corresponding to a given key, or NULL if the key
 * is not found in the table.
 */
void *table_lookup(const table *t, const void *key)
{
	// While current position is not empty, check for key match.
	int i = 0;	
	while (array_1d_has_value(t->entries, i) && i < TABLE_SIZE) {
		// Inspect table entry
		struct table_entry *entry = array_1d_inspect_value(t->entries, i);
		
		// Check if keys match in current table_entry
		if ((t->key_cmp_func(entry->key, key)) == 0) {
			// If yes, return corresponding value pointer
			return entry->value;
		}
		i++;
	}
	// No match found, return NULL.
	return NULL;
}

/**
 * table_choose_key() - Return an arbitrary key.
 * @t: Table to inspect.
 *
 * Return an arbitrary key stored in the table. Can be used together
 * with table_remove() to deconstruct the table. Undefined for an
 * empty table.
 *
 * Returns: An arbitrary key stored in the table.
 */
void *table_choose_key(const table *t)
{
	// Return first key value.
	struct table_entry *entry = array_1d_inspect_value(t->entries, 0);
	return entry->key;
}

/**
 * table_remove() - Remove a key/value pair in the table.
 * @table: Table to manipulate.
 * @key: Key for which to remove pair.
 *
 * Starts from the beginnning of the array and searches for the key/value pair 
 * to remove. Once found, freeing functions for key/value are called if 
 * specified. The last table entry in the array is moved forwards to fill the 
 * "hole" after the removed element. Does nothing if key is not found in
 * the table.
 *
 * Returns: Nothing.
 */
void table_remove(table *t, const void *key)
{
        // While current position is not empty, check for key match.
	int i = 0;	
	while (array_1d_has_value(t->entries, i) && i < TABLE_SIZE) {
		// Inspect table entry
		struct table_entry *entry = array_1d_inspect_value(t->entries, i);
		
		// Check if keys match in current table entry
		if ((t->key_cmp_func(entry->key, key)) == 0) {
			// If yes, deallocate corresponding key & value at 
			// current position
			if (t->key_free_func != NULL) {
				t->key_free_func(entry->key);
			}
			if (t->value_free_func != NULL) {
				t->value_free_func(entry->value);
			}
			// Get pointer to the last table_entry in the array
			struct table_entry *last_entry; 
			last_entry = array_1d_inspect_value(t->entries, 
							    t->index_last_pos);
			
			// Move last entry contents to fill "hole" after removed
			// element.
			entry->key = last_entry->key;
			entry->value = last_entry->value;
			
			// Set last array pos to empty and free last table entry  
			array_1d_set_value(t->entries, NULL, t->index_last_pos);
			free(last_entry);
			
			// Decrement accordingly
			t->index_last_pos--;
			return;
		}
		i++;
	}
}

/*
 * table_kill() - Destroy a table.
 * @table: Table to destroy.
 *
 * Return all dynamic memory used by the table and its elements. If a
 * free_func was registered for keys and/or values at table creation,
 * it is called each element to free any user-allocated memory
 * occupied by the element values.
 *
 * Returns: Nothing.
 */
void table_kill(table *t)
{
	// Iterate over the populated first part of the array. Destroy all 
	// elements.
	int i = 0;	
	while (array_1d_has_value(t->entries, i) && i < TABLE_SIZE) {
		
		// Inspect element
		struct table_entry *entry; 
		entry = array_1d_inspect_value(t->entries, i);
		
		// If freeing functions are specified, deallocate key/value.
		if (t->key_free_func != NULL) {
				t->key_free_func(entry->key);
		}
		if (t->value_free_func != NULL) {
			t->value_free_func(entry->value);
		}
		// Free current table_entry 
		free(entry);
		i++;
	}
	// Destroy the rest of the table structure
	array_1d_kill(t->entries);
	free(t);
}

/**
 * table_print() - Print the given table.
 * @t: Table to print.
 * @print_func: Function called for each key/value pair in the table.
 *
 * Iterates over the key/value pairs in the table and prints them.
 * Will print all stored elements, including duplicates.
 *
 * Returns: Nothing.
 */
void table_print(const table *t, inspect_callback_pair print_func)
{
	int i = 0;	
	while (array_1d_has_value(t->entries, i) && i < TABLE_SIZE) {	
		struct table_entry *e = array_1d_inspect_value(t->entries, i);	
		print_func(e->key, e->value);
	}
}

