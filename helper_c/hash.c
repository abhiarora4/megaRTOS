#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "hash.h"

// URL - https://gist.github.com/phsym/4605704
// PDF - http://www.cs.wustl.edu/~cytron/rthash.pdf

#define MAX_KEY_SIZE 32
#define MAX_DATA_SIZE 64

#define isFreeHashEntry(n,x) n##_freeNdx[x<<3] & (1<<(x & 0x07))
#define changeHashNdx(n,x,v) n##_freeNdx[x<<3] &= ~(1<< (x&0x07)); \
	n##_freeNdx[x<<3] |= (v << (x & 0x07))

typedef struct hashEntry_s {
	uint16_t hashValue;
	char key[MAX_KEY_SIZE];
	char data[MAX_DATA_SIZE];
}hashEntry_t;

typedef struct hashTable_s {
	uint16_t size;
	hashEntry_t *table;
}hashTable_t;

HASH_DEF(n,s) hashEntry_t n##_entry[s]; \
	hashTable_t n = {.size=s, .table=n##_entry}


uint16_t hashFunction(char *key)
{
	//crc16();
}

uint8_t addToHashTable(hashTable_t *hash, char *key, char *value)
{
	if(key == NULL || value == NULL)
		return -1;
	if(strlen(key) > MAX_KEY_SIZE || strlen(value) > MAX_DATA_SIZE)
		return -1;
	uint16_t hashNdx = hashFunction(key);
	//Mod Function to limit the range
	hashNdx = hashNdx % hash->size;
	uint16_t counter = 0;
	while(hash->table[hashNdx].hashValue != 0) {
		counter++;
		hashNdx++;
		hashNdx = hashNdx % hash->size;
		if(counter == hash->size)
			return -1;
	}
	strcpy((void *)hash->table[hashNdx].key, key, strnlen(key,MAX_KEY_SIZE));
	strcpy((void *)hash->table[hashNdx].data, value, strnlen(value,MAX_DATA_SIZE));
	hash->table[hashNdx].hashValue = hashFunction(key);
}

char * removeFromHash(hashTable_t *hash, char *key)
{
	uint16_t hashNdx = hashFunction(key);
	if(hash->table[hashNdx].hashValue == 0)
		return -1
	while(hash->table[hashNdx])


}
