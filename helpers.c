#include <math.h>
#include <stdio.h>
#include "helpers.h"

typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
}
RGBTEMP;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each RGBTRIPLE, find the average of each rgbt channels, set each channel to the average rounded to the nearest integer
    int average = 0;
    // For each row, i = height to i = height - 1, each pixel is j = width to j = width - 1
    for (int i = 0; i < height; i++)
    {
        // Update each pixel across each ith row
        for (int j = 0; j < width; j++)
        {
            // Calculate averages of each channel
            average = round((image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / 3.0);

            // Set each channel to average
            image[i][j].rgbtRed = average;
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each RGBTRIPLE, find the average of each rgbt channels, set each channel to the average rounded to the nearest integer
    int sepiaRed = 0;
    int sepiaGreen = 0;
    int sepiaBlue = 0;

    // For each row, i = height to i = height - 1, each pixel is j = width to j = width - 1
    for (int i = 0; i < height; i++)
    {
        // Update each pixel across each ith row
        for (int j = 0; j < width; j++)
        {
            // Calculate the sepia value of each channel
            sepiaRed = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            sepiaGreen = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            sepiaBlue = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);

            // Set each channel to sepia value, if below 255, otherwise set to 255
            if (sepiaRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = sepiaRed;
            }
            if (sepiaGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = sepiaGreen;
            }
            if (sepiaBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = sepiaBlue;
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Take in each row into a temp array stuct that matches RGBTRIPLE, write the tmp array values back into the row in reverse
    RGBTEMP tmpArr[width];
    for (int i = 0; i < height; i ++)
    {
        // Save row info into tmp array
        for (int j = 0; j < width; j++)
        {
            tmpArr[j].rgbtRed = image[i][j].rgbtRed;
            tmpArr[j].rgbtGreen = image[i][j].rgbtGreen;
            tmpArr[j].rgbtBlue = image[i][j].rgbtBlue;
        }
        // Write tmp array info back into original row in reverse
        int count = 1;
        for (int k = 0; k < width; k++)
        {
            image[i][k].rgbtRed = tmpArr[width - count].rgbtRed;
            image[i][k].rgbtGreen = tmpArr[width - count].rgbtGreen;
            image[i][k].rgbtBlue = tmpArr[width - count].rgbtBlue;
            count++;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create tmpArr[i][j] with all the blur values
    RGBTEMP tmpArr[height][width];
    for (int i = 0; i < height; i++)
    {
        // Calculations for each pixel centre
        for (int j = 0; j < width; j++)
        {
            float count = 1.0; // Keep track of the number of valid adjacent pixels
            int tmpRed = 0; // Keep track of sum of red pixels
            int tmpGreen = 0; // Keep track of sum of green pixels
            int tmpBlue = 0; // Keep track of sum of blue pixels

            // Check top left
            if (i - 1 >= 0 && j - 1 >= 0)
            {
                tmpRed += image[i - 1][j - 1].rgbtRed;
                tmpGreen += image[i - 1][j - 1].rgbtGreen;
                tmpBlue += image[i - 1][j - 1].rgbtBlue;
                count++;
            }
            // Check top middle
            if (i - 1 >= 0)
            {
                tmpRed += image[i - 1][j].rgbtRed;
                tmpGreen += image[i - 1][j].rgbtGreen;
                tmpBlue += image[i - 1][j].rgbtBlue;
                count++;
            }
            // Check top right
            if (i - 1 >= 0 && j + 1 < width)
            {
                tmpRed += image[i - 1][j + 1].rgbtRed;
                tmpGreen += image[i - 1][j + 1].rgbtGreen;
                tmpBlue += image[i - 1][j + 1].rgbtBlue;
                count++;
            }
            // Check middle left
            if (j - 1 >= 0)
            {
                tmpRed += image[i][j - 1].rgbtRed;
                tmpGreen += image[i][j - 1].rgbtGreen;
                tmpBlue += image[i][j - 1].rgbtBlue;
                count++;
            }
            // Save middle middle
            tmpRed += image[i][j].rgbtRed;
            tmpGreen += image[i][j].rgbtGreen;
            tmpBlue += image[i][j].rgbtBlue;
            // Check middle right
            if (j + 1 < width)
            {
                tmpRed += image[i][j + 1].rgbtRed;
                tmpGreen += image[i][j + 1].rgbtGreen;
                tmpBlue += image[i][j + 1].rgbtBlue;
                count++;
            }
            // Check bottom left
            if (i + 1 < height && j - 1 >= 0)
            {
                tmpRed += image[i + 1][j - 1].rgbtRed;
                tmpGreen += image[i + 1][j - 1].rgbtGreen;
                tmpBlue += image[i + 1][j - 1].rgbtBlue;
                count++;
            }
            // Check bottom middle
            if (i + 1 < height)
            {
                tmpRed += image[i + 1][j].rgbtRed;
                tmpGreen += image[i + 1][j].rgbtGreen;
                tmpBlue += image[i + 1][j].rgbtBlue;
                count++;
            }
            // Check bottom right
            if (i + 1 < height && j + 1 < width)
            {
                tmpRed += image[i + 1][j + 1].rgbtRed;
                tmpGreen += image[i + 1][j + 1].rgbtGreen;
                tmpBlue += image[i + 1][j + 1].rgbtBlue;
                count++;
            }
            // Calculate average of each with number of valid values and save to the tmpArr
            tmpArr[i][j].rgbtRed = round(tmpRed / count);
            tmpArr[i][j].rgbtGreen = round(tmpGreen / count);
            tmpArr[i][j].rgbtBlue = round(tmpBlue / count);
        }
    }
    // Write over image[i][j] with values from tmpArr[i][i]
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = tmpArr[i][j].rgbtRed;
            image[i][j].rgbtGreen = tmpArr[i][j].rgbtGreen;
            image[i][j].rgbtBlue = tmpArr[i][j].rgbtBlue;
        }
    }
    return;
}
