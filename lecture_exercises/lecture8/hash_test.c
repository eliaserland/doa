#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define TABLE_SIZE 131

unsigned int stringHash(const void *key) 
{
	char str[256];
	int length = 0;
	
	const int *int_ptr = key;
	
	length = sprintf(str, "%d", *int_ptr);
	
	if (length < 0) {
		const char *str = key;
		length = strlen(str);
	}
	
	int seed = 13131;
	unsigned int hash = 0;
	
	for (int i = 0; i < length; i++) {
		hash = (hash * seed) + str[i];
	}
	
	return hash % TABLE_SIZE;
}

int main(void) 
{
	// number of keys fed to stringHash()
	const int nkeys = 1;
	
	// create and initialize histogram
	int hash_hist[TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; i++) {
		hash_hist[i] = 0;
	}
	
	char *keys[] = {"Ja"}; //, "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	//int *keys[nkeys];
	/*for (int i = 0; i < nkeys; i++) {
		keys[i] = malloc(sizeof(int));
		*keys[i] = i;
	}*/
	
	int hash;
	int collisions = 0;		
	for (int i = 0; i < nkeys; i++) {
		hash = stringHash(keys[i]);	
		
		hash_hist[hash]++;
		
		if (hash_hist[hash] > 1 ) {
			collisions++;
		}
		//printf("Key: %d, Hash: %d \n", *keys[i], hash);
	}

	printf("Test of hash function. Size of table: %d.\n", TABLE_SIZE);	
	printf("Integer keys from 0 to %d, in total %d collisions.\n", nkeys-1, 
		collisions);
	printf("Histogram: \n\n");
	for (int i = 0; i < TABLE_SIZE; i++) {
		printf("Hash: %d, Count: %d \n", i, hash_hist[i]);
	}
	
	
	// Free memory	
	/*for (int i = 0; i < nkeys; i++) {
		free(keys[i]);
	}*/
	
	return 0;
}

