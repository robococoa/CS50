#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char rotateUppercase(char input, int k);
char rotateLowercase(char input, int k);

int main(int argc, string argv[])
{
    // Check if the input command-line is valid
    if (argc > 2 || argc == 1)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    string inputKey = argv[1];
    // Check if the input is a number
    for (int i = 0, n = strlen(inputKey); i < n; i++)
    {
        if (inputKey[i] < '0' || inputKey[i] > '9')
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }


    // Convert key to int
    int key = atoi(inputKey);

    string input = get_string("plaintext: ");
    printf("ciphertext: ");

    // Check each character of the input
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        // Check if uppercase
        if (isupper(input[i]))
        {
            printf("%c", rotateUppercase(input[i], key));
        }
        // Check if lowercase
        else if (islower(input[i]))
        {
            printf("%c", rotateLowercase(input[i], key));
        }
        // Print the input as is
        else
        {
            printf("%c", input[i]);
        }
    }
    printf("\n");
}

// Rotate uppercase letter
char rotateUppercase(char input, int k)
{
    char result = input;
    int key = k;
    // Cipher Formula: c = (((p - 65) + k) % 26) + 65, where A = 0 after subtracting 65
    result = (((input - 65) + key) % 26) + 65;

    return result;
}

// Rotate lowercase letter
char rotateLowercase(char input, int k)
{
    char result = input;
    int key = k;
    // Cipher Formula: c = (((p - 97) + k) % 26) + 65, where a = 0 after subtracting 97
    result = (((input - 97) + key) % 26) + 97;

    return result;
}
