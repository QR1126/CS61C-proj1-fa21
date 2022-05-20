#include "hashtable.h"

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *)) {
  int i = 0;
  HashTable *newTable = malloc(sizeof(HashTable));
  if (NULL == newTable) {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  newTable->size = size;
  newTable->buckets = malloc(sizeof(struct HashBucketEntry *) * size);
  if (NULL == newTable->buckets) {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  for (i = 0; i < size; i++) {
    newTable->buckets[i] = NULL;
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/* Task 1.2 */
void insertData(HashTable *table, void *key, void *data) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket location with table->hashFunction.
  // 2. Allocate a new hash bucket entry struct.
  // 3. Append to the linked list or create it if it does not yet exist.
  size_t loc = table->hashFunction(key) % table->size;
  HashBucketEntry *entry = malloc(sizeof(HashBucketEntry));
  if (entry == NULL) {
    fprintf(stderr, "malloc failed\n");
    exit(1);
  } 
  entry->key = key;
  entry->data = data;
  if (table->buckets[loc] == NULL) {
    table->buckets[loc] = entry;
  } else {
    HashBucketEntry *tmp = table->buckets[loc];
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = entry;
  }
}

/* Task 1.3 */
void *findData(HashTable *table, void *key) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket with table->hashFunction.
  // 2. Walk the linked list and check for equality with table->equalFunction.
  // 3. If a match is found, return it. If no matches are found, return NULL.
  size_t loc = table->hashFunction(key) % table->size;
  HashBucketEntry *tmp = table->buckets[loc];
  while (tmp != NULL) {
    if (table->equalFunction(key, tmp->key)) {
      return tmp->data;
    }
    tmp = tmp->next;
  }
  return NULL;
}

/* Task 2.1 */
unsigned int stringHash(void *s) {
  // -- TODO --
  // fprintf(stderr, "need to implement stringHash\n");
  /* To suppress compiler warning until you implement this function, */
  unsigned int res = 0;
  unsigned int ch = 0;
  size_t n = strlen((char*)s);
  size_t i;
  for (i = 0; i < n; i++) {
    ch = *(char*) (s + i);
    res = res * 31 + ch;
  }
  return res;
}

/* Task 2.2 */
int stringEquals(void *s1, void *s2) {
  // -- TODO --
  // fprintf(stderr, "You need to implement stringEquals");
  /* To suppress compiler warning until you implement this function */
  char *ch1 = (char*) s1;
  char *ch2 = (char*) s2;
  if (strlen(ch1) != strlen(ch2)) return 0;
  size_t n = strlen(ch1), i;
  for (i = 0; i < n; i++) {
    if (*(ch1 + i) != *(ch2 + i)) {
      return 0;
    }
  }
  return 1;
}