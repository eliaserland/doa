#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "table.h"
#include "array_1d.h"

/*
 * Implementation of a generic table for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University. Table constructed as an one dimensional array.
 *
 * Duplicates are handled by inspect and remove. FIX THIS TEXT!!!
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

// ===========INTERNAL DATA TYPES============

#define TABLE_SIZE 80000

struct table {
	array_1d *entries;
	compare_function *key_cmp_func;
	free_function key_free_func;
	free_function value_free_func;
};

struct table_entry {
	void *key;
	void *value;
};


// ===========INTERNAL FUNCTION IMPLEMENTATIONS============



/*
 *	
 *
 */
int stringHash(void *key) 
{
	char *str = (char *)key;
	int seed = 131;
	int hash = 0;
	for (int i = 0; i < strlen(str); i++) {
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
	for (int i = 0; i < TABLE_SIZE; i++) {
		if (array_1d_has_value(t->entries, i)) {
			return false;
		}
	}	
	return true;
}

/** MODIFY THIS COMMENT HERE BEFORE SUBMISSION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * table_insert() - Add a key/value pair to a table.
 * @table: Table to manipulate.
 * @key: A pointer to the key value.
 * @value: A pointer to the value value.
 *
 * Insert the key/value pair into the table. No test is performed to
 * check if key is a duplicate. table_lookup() will return the latest
 * added value for a duplicate key. table_remove() will remove all
 * duplicates for a given key.
 *
 * Returns: Nothing.
 */
void table_insert(table *t, void *key, void *value)
{
	// hash_function to find hash value of key
	int hash; //NEED HASH FUNCTION HERE
	

	// While hash position is not empty, check for key duplicate. 
	int i = 0;
	while (array_1d_has_value(t->entries, hash)) {
		
		// Inspect table entry
		struct table_entry *current_entry; 
		current_entry = array_1d_inspect_value(t->entries, hash);	
		
		// Check if keys match
		if ((t->key_cmp_func(current_entry->key, key)) == 0) {
			
			// If we have a match, call free function for the value 
			// of this entry.
			if (t->value_free_func != NULL) {
				t->value_free_func(current_entry->value);
			}
			// Set pointer to new value and exit.
			current_entry->value = value;
			return;			
		}
		// Increment hash position quadratically.
		i++;
		hash = hash + i*i;
	}
	// Current position in table is empty, allocate and insert the key/value 
	// structure at this position.
	struct table_entry *entry = malloc(sizeof(struct table_entry));	
	entry->key = key;
	entry->value = value;	
	
	array_1d_set_value(t->entries, entry, hash);		
}

/**
 * table_lookup() - Look up a given key in a table.
 * @table: Table to inspect.
 * @key: Key to look up.
 *
 * Returns: The value corresponding to a given key, or NULL if the key
 * is not found in the table. If the table contains duplicate keys,
 * the value that was latest inserted will be returned.
 */
void *table_lookup(const table *t, const void *key)
{
	// hash_function to find hash value of key
	int hash; //NEED HASH FUNCTION HERE
	
	// While hash position is not empty, check for key match.	
	while (array_1d_has_value(t->entries, hash)) {
	
		// Inspect table entry
		struct table_entry *entry; 
		entry = array_1d_inspect_value(t->entries, hash);
	
		// Check if keys match
		if ((t->key_cmp_func(entry->key, key)) == 0) {
			// If yes, return corresponding value pointer
			return entry->value;	
		}	
		// Continue and increment hash position quadratically.
		i++;
		hash = hash + i*i;
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
	// Return first key in array.
	int i = 0;
	while (!array_1d_has_value(t->entries, i)) {
		i++;
	}
	return array_1d_inspect_value(t->entries, i)->key;
}

/**
 * table_remove() - Remove a key/value pair in the table.
 * @table: Table to manipulate.
 * @key: Key for which to remove pair.
 *
 * Any matching duplicates will be removed. Will call any free
 * functions set for keys/values. Does nothing if key is not found in
 * the table.
 *
 * Returns: Nothing.
 */
void table_remove(table *t, const void *key)
{
	// hash_function to find hash value of key
	int hash; //NEED HASH FUNCTION HERE
	
	// While hash position is not empty, check for key match.	
	while (array_1d_has_value(t->entries, hash)) { 
		
		// Inspect table entry
		struct table_entry *entry; 
		entry = array_1d_inspect_value(t->entries, hash);
	
		// Check if keys match
		if ((t->key_cmp_func(entry->key, key)) == 0) {
			
			// If yes, deallocate corresponding table entry
			if (t->key_free_func != NULL) {
				t->key_free_func(entry->key);
			}
			if (t->value_free_func != NULL) {
				t->value_free_func(entry->value);
			}
			array_1d_set_value(t->entries, NULL, hash);
			free(entry);
			return;					
		}	
		// Continue and increment hash position quadratically.
		i++;
		hash = hash + i*i;	
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
		if (array_1d_has_value(t->entries, i)) {
			
			struct table_entry *entry; 
			entry = array_1d_inspect_value(t->entries, i);
					
			if (t->key_free_func != NULL) {
				t->key_free_func(entry->key);
			}
			if (t->value_free_func != NULL) {
				t->value_free_func(entry->value);
			}
			free(entry);
		}
	}
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

