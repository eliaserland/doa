#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 27

int stringHash(void *key) 
{
	char *str = (char *) key;
	int seed = 131;
	int hash = 0;

	for (int i = 0; i < strlen(str); i++) {
		hash = (hash * seed) + str[i];
	}
	return hash % TABLE_SIZE;
}

int main(void) 
{
	//char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	
	double *keys[12];
	for (int i = 0; i < 12; i++) {
		keys[i] = malloc(sizeof(double));
		*keys[i] = rand() % TABLE_SIZE;
	}
	
	//int months[] = {1,2,3,4,5,6,7,8,9,10,11,12};
	int hash;		
	for (int i = 0; i < 12; i++) {
		hash = stringHash(keys[i]);	
		printf("Key: %f, Hash: %d \n", *keys[i], hash);
	}
	
	for (int i = 0; i < 12; i++) {
		free(keys[i]);
	}
	
	return 0;
}

