#include <math.h>
#include <stdio.h>
#include <string.h>
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create tmpArr[i][j] with all the Gx and Gy values
    RGBTEMP tmpArrGx[height][width];
    memset(tmpArrGx, 0, height);
    RGBTEMP tmpArrGy[height][width];
    memset(tmpArrGy, 0, height);
    RGBTEMP tmpArr[height][width];
    memset(tmpArr, 0, height);

    for (int i = 0; i < height; i++)
    {
        // Calculations for each pixel centre
        for (int j = 0; j < width; j++)
        {
            // Check top left
            if (i - 1 >= 0 && j - 1 >= 0)
            {
                tmpArrGx[i][j].rgbtRed += -1 * image[i - 1][j - 1].rgbtRed;
                tmpArrGx[i][j].rgbtGreen += -1 * image[i - 1][j - 1].rgbtGreen;
                tmpArrGx[i][j].rgbtBlue += -1 * image[i - 1][j - 1].rgbtBlue;

                tmpArrGy[i][j].rgbtRed += -1 * image[i - 1][j - 1].rgbtRed;
                tmpArrGy[i][j].rgbtGreen += -1 * image[i - 1][j - 1].rgbtGreen;
                tmpArrGy[i][j].rgbtBlue += -1 * image[i - 1][j - 1].rgbtBlue;
            }
            // Check top middle
            if (i - 1 >= 0)
            {
                // Top middle is all zeros for Gx

                tmpArrGy[i][j].rgbtRed += -2 * image[i - 1][j].rgbtRed;
                tmpArrGy[i][j].rgbtGreen += -2 * image[i - 1][j].rgbtGreen;
                tmpArrGy[i][j].rgbtBlue += -2 * image[i - 1][j].rgbtBlue;
            }
            // Check top right
            if (i - 1 >= 0 && j + 1 < width)
            {
                tmpArrGx[i][j].rgbtRed += image[i - 1][j + 1].rgbtRed;
                tmpArrGx[i][j].rgbtGreen += image[i - 1][j + 1].rgbtGreen;
                tmpArrGx[i][j].rgbtBlue += image[i - 1][j + 1].rgbtBlue;

                tmpArrGy[i][j].rgbtRed += -1 * image[i - 1][j - 1].rgbtRed;
                tmpArrGy[i][j].rgbtGreen += -1 * image[i - 1][j - 1].rgbtGreen;
                tmpArrGy[i][j].rgbtBlue += -1 * image[i - 1][j - 1].rgbtBlue;
            }
            // Check middle left
            if (j - 1 >= 0)
            {
                tmpArrGx[i][j].rgbtRed += -2 * image[i][j - 1].rgbtRed;
                tmpArrGx[i][j].rgbtGreen += -2 * image[i][j - 1].rgbtGreen;
                tmpArrGx[i][j].rgbtBlue += -2 * image[i][j - 1].rgbtBlue;

                // Middle left is all zeros for Gy
            }
            // Middle middle is all zeros

            // Check middle right
            if (j + 1 < width)
            {
                tmpArrGx[i][j].rgbtRed += 2 * image[i][j + 1].rgbtRed;
                tmpArrGx[i][j].rgbtGreen += 2 * image[i][j + 1].rgbtGreen;
                tmpArrGx[i][j].rgbtBlue += 2 * image[i][j + 1].rgbtBlue;

                // Middle right is all zeros for Gy
            }
            // Check bottom left
            if (i + 1 < height && j - 1 >= 0)
            {
                tmpArrGx[i][j].rgbtRed += -1 * image[i + 1][j - 1].rgbtRed;
                tmpArrGx[i][j].rgbtGreen += -1 * image[i + 1][j - 1].rgbtGreen;
                tmpArrGx[i][j].rgbtBlue += -1 * image[i + 1][j - 1].rgbtBlue;

                tmpArrGy[i][j].rgbtRed += image[i + 1][j - 1].rgbtBlue;
                tmpArrGy[i][j].rgbtGreen += image[i + 1][j - 1].rgbtBlue;
                tmpArrGy[i][j].rgbtBlue += image[i + 1][j - 1].rgbtBlue;
            }
            // Check bottom middle
            if (i + 1 < height)
            {
                // Bottom middle is all zeros for Gx

                tmpArrGy[i][j].rgbtRed += 2 * image[i + 1][j].rgbtRed;
                tmpArrGy[i][j].rgbtGreen += 2 * image[i + 1][j].rgbtGreen;
                tmpArrGy[i][j].rgbtBlue += 2 * image[i + 1][j].rgbtBlue;
            }
            // Check bottom right
            if (i + 1 < height && j + 1 < width)
            {
                tmpArrGx[i][j].rgbtRed += image[i + 1][j + 1].rgbtRed;
                tmpArrGx[i][j].rgbtGreen += image[i + 1][j + 1].rgbtGreen;
                tmpArrGx[i][j].rgbtBlue += image[i + 1][j + 1].rgbtBlue;

                tmpArrGy[i][j].rgbtRed += image[i + 1][j + 1].rgbtRed;
                tmpArrGy[i][j].rgbtGreen += image[i + 1][j + 1].rgbtGreen;
                tmpArrGy[i][j].rgbtBlue += image[i + 1][j + 1].rgbtBlue;
            }
            // Calculate positive values and save to the tmpArr
            int tmpRedGx = exp2(tmpArrGx[i][j].rgbtRed);
            int tmpRedGy = exp2(tmpArrGy[i][j].rgbtRed);
            int tmpRed = sqrt(tmpRedGx + tmpRedGy);
            int tmpGreenGx = exp2(tmpArrGx[i][j].rgbtGreen);
            int tmpGreenGy = exp2(tmpArrGy[i][j].rgbtGreen);
            int tmpGreen = sqrt(tmpGreenGx + tmpGreenGy);
            int tmpBlueGx = exp2(tmpArrGx[i][j].rgbtBlue);
            int tmpBlueGy = exp2(tmpArrGy[i][j].rgbtBlue);
            int tmpBlue = sqrt(tmpBlueGx + tmpBlueGy);

            tmpArr[i][j].rgbtRed = round(tmpRed);
            //printf("tmparr red %i = sqrt of %i (%i^2 + %i^2)", tmpArr[i][j].rgbtRed, tmpRed, tmpArrGx[i][j].rgbtRed, tmpArrGy[i][j].rgbtRed);
            tmpArr[i][j].rgbtGreen = round(tmpGreen);
            tmpArr[i][j].rgbtBlue = round(tmpBlue);
        }
    }
    // Write over image[i][j] with values from tmpArr[i][i]
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Red
            if (tmpArr[i][j].rgbtRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = tmpArr[i][j].rgbtRed;
            }
            // Green
            if (tmpArr[i][j].rgbtGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = tmpArr[i][j].rgbtGreen;
            }
            // Blue
            if (tmpArr[i][j].rgbtBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = tmpArr[i][j].rgbtBlue;
            }
        }
    }
    return;
}
