#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int countLetters(string input);
int countWords(string input);
int countSentences(string input);

int main(void)
{
    string input = get_string("Text: ");
    int letters = countLetters(input);
    int words = countWords(input);
    int sentences = countSentences(input);
    float L = 0;
    float S = 0;
    int index = 0;

    // Calculate L
    L = (letters / (float) words) * 100;

    // Calculate S
    S = (sentences / (float) words) * 100;

    // Calculate index
    index = round((0.0588 * L) - (0.296 * S) - 15.8);

    // printf("Letters: %i\nWords: %i\nSentences: %i\n", letters, words, sentences);

    // Return Grade calculation
    if (index < 1)
    {
        printf("Before Grade 1");
    }
    else if (index >= 16)
    {
        printf("Grade 16+");
    }
    else
    {
        printf("Grade %i", index);
    }
}

// Calculate letter count
int countLetters(string input)
{
    int count = 0;

    // Count each valid letter
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if ((input[i] >= 'A' && input[i] <= 'Z') || (input[i] >= 'a' && input[i] <= 'z'))
        {
            count++;
        }
    }
    return count;
}

// Calculate word count
int countWords(string input)
{
    int count = 1;

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        // Check for the end of each word
        if (input[i] == ' ')
        {
            count++;
        }
    }
    return count;
}

// Calculate sentence count
int countSentences(string input)
{
    int count = 0;

    // Count each sentence ending in '.', '!', or '?'
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (input[i] == '.' || input[i] == '!' || input[i] == '?')
        {
            count++;
        }
    }
    return count;
}