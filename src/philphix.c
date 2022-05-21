/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
#ifndef _PHILPHIX_UNITTEST
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}
#endif /* _PHILPHIX_UNITTEST */

/* Task 3 */
void readDictionary(char *dictName) {
  // -- TODO --
  // fprintf(stderr, "You need to implement readDictionary\n");
  // HINT:
  // 1. Open the specified file. If the file doesn't exist, 
  // print a message to standard error and call exit(61) to cleanly exit the program.
  FILE *fp;
  fp = fopen(dictName, "r");
  if (fp == NULL) {
    fprintf(stderr, "open file doesn't exist\n");
    exit(61);
  }
  // 2. Read each word, one at a time, and insert each key/value pair into the dictionary. 
  // Since words can be any length, you probably need to read characters from the file 
  // one at a time.
  const int size = 1000000;
  while (1) {
    char *key = (char*) malloc(sizeof(char) * size);
    char *value = (char*) malloc(sizeof(char) * size);
    if (fscanf(fp, "%s%s", key, value) == EOF) {
      break;
    }
    if (strlen(key) && strlen(value)) insertData(dictionary, key, value);
  }
  fclose(fp);
}

/* Task 4 */
void processInput() {
  // -- TODO --
  // fprintf(stderr, "You need to implement processInput\n");
  // HINT:
  // 1. Read non-alphanumeric characters from stdin and print it unchanged to stdout. 
  // To preserve non-alphanumeric characters, you probably need to read characters from 
  // stdin one at a time.
  // 2. Read words from stdin. If the word or a variation appears as a key in the dictionary, 
  // print the replacement word (stored as the data corresponding to the key in the hash table) 
  // to stdout. Remember to follow the order of checking variations: exact word, 
  // then all but first character lowercase, then all lowercase.
  const int size = 1000000;
  char *word = (char*) malloc(sizeof(char) * size);
  int idx = 0;
  char c;
  while ((c = getchar()) != EOF || strlen(word) != 0) {
    if (isalpha(c) || isdigit(c)) {
      *(word + idx++) = c;
      continue;
    }

    *(word + idx) = '\0';
    idx = 0;
    // if the exact word appears as key in the dictionary
    char *replace_word = (char *) findData(dictionary, word);
    if (replace_word != NULL) {
      printf("%s", replace_word);
      if (c != EOF) printf("%c", c);
      // free the pointer to word
      memset(word, 0, sizeof(word));
      continue;
    }

    // if all but first character lowercase appears 
    char *tmp = (char*) malloc(sizeof(char) * size);
    strcpy(tmp, word);
    int i = 1;
    for (; i < strlen(tmp); i++) {
      *(tmp + i) = tolower(*(tmp + i));
    }
    if ((replace_word = findData(dictionary, tmp)) != NULL) {
      printf("%s", replace_word);
      if (c != EOF) printf("%c", c);
      memset(word, 0, sizeof(word));
      free(tmp);
      continue;
    }

    // if all character lowercase appears
    *tmp = tolower(*tmp);
    if ((replace_word = findData(dictionary, tmp)) != NULL) {
      printf("%s", replace_word);
      if (c != EOF) printf("%c", c);
      memset(word, 0, sizeof(word));
      free(tmp);
      continue;
    }

    printf("%s", word);
    if (c != EOF) printf("%c", c);
    memset(word, 0, sizeof(word));
    free(tmp);
  }

  free(word);
}