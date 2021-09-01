#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    void pyramidBricks(int i);

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
        // Print left side of pyramid
        pyramidBricks(i);

        //Print middle gap
        printf("  ");

        // Print right side of pyramid
        pyramidBricks(i);
        printf("\n");
    }
}

// Function to print pyramid bricks
void pyramidBricks(int i)
{
    for (int j = 0; j <= i; j++)
    {
        printf("#");
    }
}