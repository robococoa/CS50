#include <stdio.h>
#include <cs50.h>
#include <math.h>

int howManyCoins(int changeRemaining, int value);

int main(void)
{
    // Prompt user for required change
    float change;
    int changeRemaining;
    do
    {
        change = get_float("Change owed: ");

        // Convert required change to cents
        changeRemaining = round(change * 100);
    }
    while (changeRemaining < 1);

    // Calculate the number of required coins
    int coins = 0;
    int coinsUsed = 0;

    while(changeRemaining > 0)
    {
        // Check if quarters can be used
        coinsUsed = howManyCoins(changeRemaining, 25);
        coins += coinsUsed;
        changeRemaining -= coinsUsed * 25;
        coinsUsed = 0;

        // Check if dimes can be used
        coinsUsed = howManyCoins(changeRemaining, 10);
        coins += coinsUsed;
        changeRemaining -= coinsUsed * 10;
        coinsUsed = 0;

        // Check if nickels can be used
        coinsUsed = howManyCoins(changeRemaining, 5);
        coins += coinsUsed;
        changeRemaining -= coinsUsed * 5;
        coinsUsed = 0;

        // Check if pennies can be used
        coinsUsed = howManyCoins(changeRemaining, 1);
        coins += coinsUsed;
        changeRemaining -= coinsUsed;
    }


    printf("%i\n", coins);
}

// Calculate value to reduce remaining change
int howManyCoins(int changeRemaining, int coinSize)
{
    int coins = 0;
    int changeToCheck = changeRemaining;
    do
    {
        if (changeToCheck - coinSize >= 0)
        {
            changeToCheck -= coinSize;
            coins++;
        }
    }
    while (changeToCheck - coinSize >= 0);

    return coins;
}