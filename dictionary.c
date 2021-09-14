// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Global Variables
int dictionarySize = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash reference for word
    int position = hash(word);
    //printf("position: %i\n", position);                                                                              // debugging
    node *cursor = table[position];
    //printf("table[position]->word %s\n", table[position]->word);                                                                              // debugging
    //printf("cursor->word: %s\n", cursor->word);                                                                              // debugging
    // Check if the ->word in each position matches the input word
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            //printf("found a match, return true\n");                                                                                                       // debugging
            return true;
        }
        else
        {
            //printf("no match found, set cursor to new target and check again\n");                                                                              // debugging
            cursor = cursor->next;
        }
    }

    //printf("no match found, return check as false\n");                                                                                                        // debugging
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Convert first letter of word into 0-25 for the letter of the alphabet that denotes the start of the word
    int key = (tolower(word[0]) - 97) % N;
    //printf("### Hashing ### word was: %s, key is: %i\n", word, key);                                                                                          // debugging
    return key;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Read dictionary into memory
    FILE *file = fopen(dictionary, "r");
    // Check for valid file
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file.\n");
        return false;
    }

    // Load each line of the dictionary until file end
    char word[64];
    //char *word = malloc(sizeof(char) * LENGTH);
    while(fscanf(file, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Could not allocate memory for word.");
            return false;
        }
        // Copy the dictionary word into the node
        strcpy(n->word, word);
        // Determine position in array from hash of word
        int position = hash(word);
        // If the table array position is 0/NULL, add this word as the pointer, if not, insert into the linked list
        if (table[position] == NULL)
        {
            //printf("table[position] is null, set table to n\n");                                                                                                          // debugging
            table[position] = n;
            //printf("table[position] should now have been set: %s\n", table[position]->word);                                                                              // debugging
        }
        else
        {
            //printf("table[position] is not null, switch\n");                                                                                                              // debugging
            n->next = table[position];
            table[position] = n;
            //  printf("table[position] should now have been swapped to n: %s, with next position updated to: %s\n", table[position]->word, table[position]->next->word);       //debugging
        }
        dictionarySize++;
        //printf("new word added: %s, table[position]->word = %s\n", word, table[position]->word);                                                                              // debugging
    }

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Returns the global variable, size, prepopulated by the hash function
    return dictionarySize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Cycle through table[]
    for (int i = 0; i < 26; i++)
    {
        node *cursor = table[i];
        node *tmp = table[i];
        while(cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
