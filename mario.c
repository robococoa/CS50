#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;

    // Prompt user for the height of the pyramid
    do
    {
        height = get_int("Height: ");
    }
    while (height > 8 || height < 1);

    // Print the pyramid
    for (int i = 0; i < height; i++)
    {
        // Print spaces
        for (int x = height - i; x > 1; x--)
        {
            printf(" ");
        }
        // Print hashes
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}