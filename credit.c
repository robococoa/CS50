#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>

bool checkSum(string cardNumber);

int main(void)
{
    string cardType = "INVALID";
    string cardNumber = get_string("Number: ");
    int length = strlen(cardNumber);

    // Check which card provider the format is for
    // Check if the number is a American Express format
    if (strncmp(cardNumber, "34", 2) == 0 || strncmp(cardNumber, "37", 2) == 0)
    {
        if (length == 15)
        {
            if (checkSum(cardNumber))
            {
                cardType = "AMEX";
                printf("%s\n", cardType);
            }
            else
            {
                printf("%s\n", cardType);
            }
        }
    }
    // Check if the number is a Mastercard format
    else if (strncmp(cardNumber, "51", 2) == 0 || strncmp(cardNumber, "52", 2) == 0 || strncmp(cardNumber, "53", 2) == 0 || strncmp(cardNumber, "54", 2) == 0 || strncmp(cardNumber, "55", 2) == 0)
    {
        if (length == 16)
        {
            if (checkSum(cardNumber))
            {
                cardType = "MASTERCARD";
                printf("%s\n", cardType);
            }
            else
            {
                printf("%s\n", cardType);
            }
        }
    }
    // if number is 13 or 16, and starts with 4, continue with VIA checks
    else if (strncmp(cardNumber, "4", 1) == 0)
    {
        printf("Checking if VISA\n");
        if (length == 13 || length == 16)
        {
            if (checkSum(cardNumber))
            {
                cardType = "VISA";
                printf("%s\n", cardType);
            }
            else
            {
                printf("%s\n", cardType);
            }
        }
        else
        {
            printf("%s\n", cardType);
        }
    }
    else
    {
        printf("%s\n", cardType);
    }
}

// Perform checksum on card number
bool checkSum(string cardNumber)
{
    // Convert card number into a long
    char *endPtr;
    long card = strtol(cardNumber, &endPtr, 10);
    int digit;
    int count = 1;
    int multiplied;
    int sumOfDigits = 0;
    int sumOfOtherDigits = 0;

    // Check each digit starting from the end
    while (card > 10)
    {
        digit = card % 10;
        // printf("card digit: %i\n", digit);
        card /= 10;
        // Multiply every 2nd digit by 2, and add to a summation
        if (count % 2 == 0)
        {
            // printf("add this digit to sum.\n");
            multiplied = digit * 2;
            // printf("multiplied: %i\n", multiplied);
            // Separate the digits out if there's more than 1, before adding to the sum
            if (multiplied / 10 > 0)
            {
                // printf("value has more than 1 digit.\n");
                int multipliedDigits = multiplied;
                while (multipliedDigits > 9)
                {
                    sumOfDigits += multiplied % 10;
                    // printf("sum of digits: %i\n", sumOfDigits);
                    multipliedDigits /= 10;
                }
                sumOfDigits += multipliedDigits;
                // printf("sum of digits: %i\n", sumOfDigits);
            }
            // If there is only 1 digit, add it to the sum
            else
            {
                sumOfDigits += multiplied;
                // printf("sum of digits: %i\n", sumOfDigits);
            }
        }
        // Keep a track of the sum of the alternate digits
        else
        {
            sumOfOtherDigits += digit;
            // printf("sum of other digits: %i\n", sumOfOtherDigits);
        }
        count++;
        // printf("count: %i\n", count);
        // printf("card: %li\n", card);
    }
    // printf("exited while loop\n");
    // Add first digit to appropriate sum
    if (count % 2 == 0)
    {
        if (card == 5)
        {
            sumOfDigits++;
            // printf("sum of digits: %i\n", sumOfDigits);
        }
        else
        {
            sumOfDigits += card * 2;
            // printf("sum of digits: %i\n", sumOfDigits);
        }
    }
    else
    {
        sumOfOtherDigits += card;
        // printf("sum of other digits: %i\n", sumOfOtherDigits);
    }
    // Add the remaining digits from the card number to the summation
    sumOfDigits += sumOfOtherDigits;
    // printf("sum of digits and other digits: %i\n", sumOfDigits);
    // If the last digit is zero, it passes the checksum
    if (sumOfDigits % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}