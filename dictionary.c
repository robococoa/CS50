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
    node *cursor = table[position];
    // Check if the ->word in each position matches the input word
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Convert first letter of word into 0-25 for the letter of the alphabet that denotes the start of the word
    int key = (tolower(word[0]) - 97) % N;
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
        // If the table array position is NULL, add this word as the pointer, if not, insert into the linked list
        if (table[position] == NULL)
        {
            table[position] = n;
        }
        else
        {
            n->next = table[position];
            table[position] = n;
        }
        dictionarySize++;
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
        while(cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
