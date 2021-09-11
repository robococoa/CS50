#include <cs50.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Declarations
const int HEADER_SIZE = 4;
const int BLOCK_SIZE = 512;
const int FILENAME_LENGTH = 3;
typedef uint8_t BYTE;

// Prototypes
bool checkIfJPEG(BYTE buffer[]);

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

    // Search for JPEG header, the first 4 bytes of each 512 byte block
    // Save each 512 byte block into an array to allow checking of the 1st 4 bytes
    BYTE buffer[BLOCK_SIZE];
    bool isOpen = false;
    char *filename = malloc(FILENAME_LENGTH);
    int outputNumber = 0;
    sprintf(filename, "%03i.jpg", outputNumber);
    FILE *img = fopen(filename, "w");

    while (fread(&buffer, sizeof(BYTE), BLOCK_SIZE, file))
    {

        if (checkIfJPEG(buffer))
        {
            // If a JPEG header is found, open a new output file if isOpen is false, and set isOpen to true
            if (isOpen == false)
            {
                isOpen = true;

                if (img == NULL)
                {
                    printf("Could not open file.\n");
                    return 1;
                }
                // Write the buffer to the new file
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, img);

                // Update filename string
                outputNumber++;
                // Re-set filename to incremented value
                sprintf(filename, "%03i.jpg", outputNumber);
            }
            // If isOpen is true, a new JPEG header is found, don't write, but close the output file, need a new file before writing this block
            else if (isOpen == true)
            {
                fclose(img);
                isOpen = false;
                // Open new file with updated filename string
                fopen(filename, "w");
                if (img == NULL)
                {
                    printf("Could not open file.\n");
                    return 1;
                }
                // Write the buffer to the new file
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, img);
                // Update filename string
                outputNumber++;
                // Re-set filename to incremented value
                sprintf(filename, "%03i.jpg", outputNumber);
            }
        }
        else
        {
            // Write the buffer to the open file
            fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, img);
        }
    }
}

bool checkIfJPEG(BYTE buffer[])
{
    if (buffer[0] == 0xff)
    {
        if (buffer[1] == 0xd8)
        {
            if (buffer[2] == 0xff)
            {
                if ((buffer[3] & 0xf0) == 0xe0)     // bit arithmetic to zero out 2nd byte since all options are valid
                {
                    return true;
                }
            }
        }
    }
    return false;
}
