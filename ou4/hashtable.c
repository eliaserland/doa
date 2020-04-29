#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "table.h"
#include "array_1d.h"

/*
 * Implementation of a generic hash table for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University. Table constructed as an one dimensional array where index is 
 * found using a simple hash function. Hash collisions are handled by closed,
 * quadratically incremented searches.
 *
 * Duplicates are handled at element insertion. A "removed" marker will be 
 * placed in the array at element removal.
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
#define TABLE_SIZE 80021

// ==================INTERNAL DATA TYPES====================

struct table {
	array_1d *entries;
	compare_function *key_cmp_func;
	free_function key_free_func;
	free_function value_free_func;
	int nr_of_elements;
};

struct table_entry {
	void *key;
	void *value;
};

// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
 * hash_function() - Hash the given key to an useable array index.
 * @key: A pointer to the key value. Key has to be of int type or char array 
 *	 with at least 4 characters. Only the first 4 bytes of the key will be 
 *	 used in the hashing process.	
 * 
 * Returns: Array index corresponding to the key value.  
 */
unsigned int hash_function(const void *key) 
{
	// Allocate string buffer and length of string.
	char str[16];
	int length = 0;
	
	// Typecast the key to int, only "keeping" the first 4 bytes of data.
	const int32_t *int_ptr = key;
	
	// Convert int to a string of numbers, save the length of the string.
	length = sprintf(str, "%d", *int_ptr);
	
	// Hash the string.
	unsigned int hash = 0;
	int seed = 13131; // Magic number
	for (int i = 0; i < length; i++) {
		hash = (hash * seed) + str[i];
	}
	return hash % TABLE_SIZE;
}

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
	t->nr_of_elements = 0;
	
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
	return t->nr_of_elements == 0;
}

/**
 * table_insert() - Add a key/value pair to a table.
 * @table: Table to manipulate.
 * @key: A pointer to the key value.
 * @value: A pointer to the value value.
 *
 * Insert the key/value pair into the table. A call upon the hash function 
 * generates an index in the array for the given key. Checks for key duplicates
 * and previously populated elements are performed for the position given by the
 * hash. Duplicates are removed and collisions are dealt with closed, quadratic 
 * increments of the position in the array.
 *
 * Returns: Nothing.
 */
void table_insert(table *t, void *key, void *value)
{
	// hash_function to find hash value of key
	unsigned int hash = hash_function(key); 

	// While hash position is not empty, check for "removed key" marker and
	// key duplicate. 
	int i = 0;
	while (array_1d_has_value(t->entries, hash)) {
		
		// Inspect table entry
		struct table_entry *current_entry; 
		current_entry = array_1d_inspect_value(t->entries, hash);
		
		// Check for "removed" marker, i.e. current table_entry has
		// key and value pointers both set to NULL.    	
		if (current_entry->key == NULL && current_entry->value == NULL) {
			// This position is free, but has previously held an
			// element. Set pointers, increment element count and 
			// exit.
			current_entry->key = key;
			current_entry->value = value;
			t->nr_of_elements++;
			return;  
		}
		
		// Check if keys match in current table_entry
		if ((t->key_cmp_func(current_entry->key, key)) == 0) {
			// If we have a key match, call free function for the 
			// old key/value pair of this entry. 
			if (t->key_free_func != NULL) {
				t->key_free_func(current_entry->key);
			}
			if (t->value_free_func != NULL) {
				t->value_free_func(current_entry->value);
			}
			// Set pointer to new key/value pointers and exit.
			current_entry->key = key;
			current_entry->value = value;
			return;			
		}
		// Increment hash position quadratically.
		i++;
		hash = (hash + i*i) % TABLE_SIZE;
	}
	// Current position in table is empty, allocate and insert new key/value 
	// structure at this position.
	struct table_entry *entry = malloc(sizeof(struct table_entry));	
	entry->key = key;
	entry->value = value;	
	
	// Set pointer to table entry in array and increment element count.
	array_1d_set_value(t->entries, entry, hash);
	t->nr_of_elements++;		
}

/**
 * table_lookup() - Look up a given key in a table.
 * @table: Table to inspect.
 * @key: Key to look up.
 *
 * A call to the hash function generates an index corresponding to the key 
 * value. Until the matching key is found or we reach an empty array element, 
 * increment the hash position quadratically, checking key values and 
 * skipping over "removed" markers. 
 * 
 * Returns: The value corresponding to a given key, or NULL if the key
 * is not found in the table.
 */
void *table_lookup(const table *t, const void *key)
{
	// hash_function to find hash value of key
	unsigned int hash = hash_function(key);
	
	// While hash position is not empty, check for key match.
	int i = 0;	
	while (array_1d_has_value(t->entries, hash)) {
	
		// Inspect table entry
		struct table_entry *entry; 
		entry = array_1d_inspect_value(t->entries, hash);
		
		// Check that the current position does not have the "removed" 
		// marker. Key/value pointers = NULL indicates removed 
		// element. 
		if (entry->key != NULL && entry->value != NULL) {
			// Check if keys match
			if ((t->key_cmp_func(entry->key, key)) == 0) {
				// If yes, return corresponding value pointer
				return entry->value;	
			}	
		}
		
		// Continue and increment hash position quadratically.
		i++;
		hash = (hash + i*i) % TABLE_SIZE;
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
	// Search for the first non-NULL key in the array.
	int i = 0;
	bool searching = true;
	struct table_entry *entry;
	
	while (searching) {	
		// Check if current position is nonempty
		if (array_1d_has_value(t->entries, i)) {
		
			// Inspect table entry
			entry = array_1d_inspect_value(t->entries, i);
			
			// Check if element has the removed marker
			if (entry->key != NULL && entry->value != NULL) {
				// Current element is legit, exit the search.
				searching = false;
			} else {
				// Current element has the removed marker, 
				// continue the search.
				i++;
			}
		} else {
			// Current position in array is empty, continue search.
			i++;
		}
	}
	// Return the key.
	return entry->key;
}

/**
 * table_remove() - Remove a key/value pair in the table.
 * @table: Table to manipulate.
 * @key: Key for which to remove pair.
 *
 * Places a "removed" marker at the position of the key/value pair. Will call 
 * any free functions set for keys/values. Does nothing if key is not found in
 * the table.
 *
 * Returns: Nothing.
 */
void table_remove(table *t, const void *key)
{
	// hash_function to find hash value of key
	unsigned int hash = hash_function(key);
	
	// While hash position is not empty, check for key match.
	int i = 0;	
	while (array_1d_has_value(t->entries, hash)) { 
		
		// Inspect table entry
		struct table_entry *entry; 
		entry = array_1d_inspect_value(t->entries, hash);
		
		// Check if current element has the "removed" marker, if yes 
		// skip this element and continue with the search.
		if (entry->key != NULL && entry->value != NULL) {
		
			// Check if keys match
			if ((t->key_cmp_func(entry->key, key)) == 0) {
				
				// If yes, deallocate corresponding key & value
				if (t->key_free_func != NULL) {
					t->key_free_func(entry->key);
				}
				if (t->value_free_func != NULL) {
					t->value_free_func(entry->value);
				}
				
				// Leave the table_entry, but set key and value 
				// pointers to NULL. This is the indication of a
				// removed element.
				entry->key = NULL; 
				entry->value = NULL;
				
				// Decrement count of table elements
				t->nr_of_elements--;
				return;					
			}	
		}
		// Continue the search and increment hash position quadratically
		i++;
		hash = (hash + i*i) % TABLE_SIZE;	
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
	for (int i = 0; i < TABLE_SIZE; i++) {
		// Check if for nonempty array elements
		if (array_1d_has_value(t->entries, i)) {
			
			// Inspect element
			struct table_entry *entry; 
			entry = array_1d_inspect_value(t->entries, i);
			
			// If freeing functions are sepcified and key/value 
			// pointers are not NULL, deallocate key/value.		
			if (t->key_free_func != NULL && entry->key != NULL) {
				t->key_free_func(entry->key);
			}
			if (t->value_free_func != NULL && entry->value != NULL) {
				t->value_free_func(entry->value);
			}
			// Free current table_entry 
			free(entry);
		}
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
	for (int i = 0; i < TABLE_SIZE; i++) {
		if (array_1d_has_value(t->entries, i)) {
			struct table_entry *e = array_1d_inspect_value(t->entries, i);	
			print_func(e->key, e->value);
		}
	}
}

