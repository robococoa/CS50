#include <math.h>
#include <stdio.h>
#include "helpers.h"

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
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
