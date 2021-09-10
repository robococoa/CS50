#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check for 2 command line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    // Filename of input image
    char *image = argv[1];

    // Open input file
    FILE *file = fopen(argv[1], "r");

    // Check for valid file
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", image);
        return 1;
    }
}