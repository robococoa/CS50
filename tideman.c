#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void record_preferences_recursive(int ranks[], int size);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // The chosen candidate, name, is inserted as an int (i) into the ranks[] array
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int arrLength = 0;
    // For the ith candidate in candidates[], being the ith candidate in preferences[], increment any candidate that is lower in ranks[i+n]
    for (int i = 0; i < candidate_count; i++)
    {
        // Determine ith candidate in ranks[] (ranks[i] is an int, which equals candidates[ranks[i]], which is the same position in preferences[ranks[i]])
        // Set 0 for the themself-themself pair (ie. ranks[i] where i = 0, in preferences[ranks[0]][ranks[0]] = 0)
        if (i == 0)
        {
            preferences[ranks[0]][ranks[0]] = 0;
        }
        // For preferences[ranks[0]], iterate over the remainder of ranks[] and increment preferences[] for that candidate (preferences[ranks[i]][j])
        else
        {
            preferences[ranks[0]][ranks[i]]++;
            arrLength = i;
        }
    }
    // Recursively iterate through the remaining ranks[], sending a reduced ranks[] as new input
    // Create new ranks[] if another recursion can be performed, using the current rank[] but dropping off the 1st element
    if (arrLength > 0)
    {
        int remainingRanks[arrLength];
        for (int j = 0; j < arrLength; j++)
        {
            remainingRanks[j] = ranks[j + 1];
        }
        // int size = sizeof(remainingRanks);
        record_preferences_recursive(remainingRanks, arrLength);
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Check each candidate in the preferences array
    for (int i = 0; i < candidate_count; i++)
    {
        // Check each pair, starting with 0 and 1, then checking 1 and 0 to see if there is a winner
        // Save the value of votes for each, then compare
        for (int j = 1; j < candidate_count - i; j++)
        {
            int person1 = preferences[i][j + i];
            int person2 = preferences[j + i][i];

            if (person1 > person2)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j + i;
                pair_count++;
            }
            else if (person2 > person1)
            {
                pairs[pair_count].winner = j + i;
                pairs[pair_count].loser = i;
                pair_count++;
            }
            // else pairs is not updated if equal
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int firstPairStrength = 0;
    int secondPairStrength = 0;
    int temp[2];
    // Sort by highest strength of win (winning value - losing value)
    for (int i = 0; i < pair_count - 1; i++)
    {
        // Reverse bubble sort, starting with pair 0
        if (i == 0)
        {
            firstPairStrength = (preferences[pairs[i].winner][pairs[i].loser]) - (preferences[pairs[i].loser][pairs[i].winner]);
        }
        // Compare with next pair
        if (i > 0)
        {
            secondPairStrength = (preferences[pairs[i].winner][pairs[i].loser]) - (preferences[pairs[i].loser][pairs[i].winner]);
            // Swap pairs if out of descending order
            if (firstPairStrength < secondPairStrength)
            {
                temp[0] = pairs[i - 1].winner;
                temp[1] = pairs[i - 1].loser;
                pairs[i - 1].winner = pairs[i].winner;
                pairs[i - 1].loser = pairs[i].loser;
                pairs[i].winner = temp[0];
                pairs[i].loser = temp[1];
            }
            // Reset firstPairStrength to current position
            firstPairStrength = secondPairStrength;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Keep track of the number of true values for each candidate
    int candidateLosses[MAX] = {0};
    for (int i = 0; i < pair_count; i++)
    {
        // Each loser or pairs[i].loser is to be added to locked[] as being beaten by winner or pairs[i].winner, as true
        if (i == 0)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            candidateLosses[pairs[i].loser]++;
        }

        int count = 0;
        // If the loser is for the last candidate with no losses, skip
        if (i > 0)
        {
            for (int j = 0; j < candidate_count; j++)
            {
                if (candidateLosses[j] > 0)
                {
                    count++;
                }
            }
            // Check if the cycle could be completed
            if (count == pair_count - 1)
            {
                // Check if the final open edge is the same as the new loser
                for (int j = 0; j < candidate_count; j++)
                {
                    if (candidateLosses[j] == 0 && candidateLosses[j] == pairs[i].loser)
                    {
                        // Do not update this, as it will close the cycle graph
                    }
                }
                locked[pairs[i].winner][pairs[i].loser] = true;
                candidateLosses[pairs[i].loser]++;
            }
            else
            {
                locked[pairs[i].winner][pairs[i].loser] = true;
                candidateLosses[pairs[i].loser]++;
            }
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int candidateLossCount[candidate_count];
    int count = candidate_count;

    // Winner has the lowest number of true values

    // Check for outright winner
    int j = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        int hasWonOutright = 0;
        if (locked[i][j] == true)
        {
            candidateLossCount[i]++;
        }
        else
        {
            hasWonOutright++;
        }
        j++;

        if (hasWonOutright == 3)
        {
            printf("%s\n", candidates[i]);
            return;
        }
        else
        {
            hasWonOutright = 0;
        }
    }
    // If no outright winner, find candidates with the lowest losses
    for (int i = 0; i < candidate_count; i++)
    {
        for (int k = 0; k < candidate_count; k++)
        {
            if (candidateLossCount[k] < count)
            {
                count = candidateLossCount[k];
            }
        }
        for (int l = 0; l < candidate_count; l++)
        {
            if (candidateLossCount[l] == count)
            {
                printf("%s\n", candidates[l]);
            }
        }
    }
    return;
}

// Get the size of an array
void record_preferences_recursive(int ranks[], int size)
{
    int arrLength = size;
    int remainingArrLength = 0;
    // For the ith candidate in candidates[], being the ith candidate in preferences[], increment any candidate that is lower in ranks[i+n]
    for (int i = 0; i < arrLength; i++)
    {
        // Determine ith candidate in ranks[] (ranks[i] is an int, which equals candidates[ranks[i]], which is the same position in preferences[ranks[i]])
        // Set 0 for the themself-themself pair (ie. ranks[i] where i = 0, in preferences[ranks[0]][ranks[0]] = 0)
        if (i == 0)
        {
            preferences[ranks[0]][ranks[0]] = 0;
        }
        // For preferences[ranks[0]], iterate over the remainder of ranks[] and increment preferences[] for that candidate (preferences[ranks[i]][j])
        else
        {
            preferences[ranks[0]][ranks[i]]++;
            remainingArrLength = i;
        }
    }
    // Recursively iterate through the remaining ranks[], sending a reduced ranks[] as new input
    // Create new ranks[] if another recursion can be performed, using the current rank[] but dropping off the 1st element
    if (remainingArrLength > 0)
    {
        int remainingRanks[remainingArrLength];
        for (int j = 0; j < remainingArrLength; j++)
        {
            remainingRanks[j] = ranks[j + 1];
        }
        // int length = sizeof(remainingRanks);
        record_preferences_recursive(remainingRanks, remainingArrLength);
    }
    return;
}