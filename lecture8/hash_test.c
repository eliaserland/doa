#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 23

int stringHash(char *str) 
{
	int seed = 131;
	int hash = 0;

	for (int i = 0; i < strlen(str); i++) {
		hash = (hash * seed) + str[i];
	}
	return hash % TABLE_SIZE;
}

int main(void) 
{
	char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", 
			"Sep", "Oct", "Nov", "Dec"};
	int hash;		
	for (int i = 0; i < 12; i++) {
		hash = stringHash(months[i]);	
		printf("String: %s, Hash: %d \n", months[i], hash);
	}	
	
	return 0;
}

