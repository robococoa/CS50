#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int substitute(char input);

int main(int argc, string argv[])
{
    // Ensure the command-line argument is valid
    char substitutionKey[26];
    if (argc < 2 || argc > 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    if (strlen(argv[1]) != 26)
    {
        printf("Error: Key must contain 26 characters.\n");
        return 1;
    }
    // Check key contains 26 alphabetic characters
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Error: Key must consist of 26 alphabet characters.\n");
            return 1;
        }
        // If alphabetic, insert into a reference substitution key
        substitutionKey[i] = argv[1][i];
    }
    // Check if all key letters are unique
    for (int i = 0; i < 26; i++)
    {
        // printf("\ncomparing: %c", substitutionKey[i]);
        for (int j = (i + 1); j < 26; j++)
        {
            // printf(" with %c", substitutionKey[j]);
            if (substitutionKey[i] == substitutionKey[j])
            {
                printf("Error: Key must consist of 26 unique characters.\n");
                return 1;
            }
        }
    }

    string key = argv[1];
    string input = get_string("plaintext:  ");

    // Convert and print each character in input
    printf("ciphertext: ");
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (isalpha(input[i]))
        {
            int position = substitute(input[i]);
            // Handle lowercase input, so that the output matches
            if (islower(input[i]))
            {
                printf("%c", tolower(substitutionKey[position]));
            }
            else
            {
                printf("%c", toupper(substitutionKey[position]));
            }

        }
        else
        {
            printf("%c", input[i]);
        }
    }
    printf("\n");
}

// Substitution input character
int substitute(char input)
{
    int position;
    if (isupper(input))
    {
        position = input - 65;
    }
    else
    {
        position = input - 97;
    }
    return position;
}