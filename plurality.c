#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // Compare input name to all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // If candidate is found, add 1 vote to that candidate
        if (strcmp(name, candidates[i].name) == 0)
        {
            // printf("%s matches %s\n", name, candidates[i].name);
            candidates[i].votes++;
            return true;
        }
    }
    // If no candidate is found, exit
    // printf("No matches found\n");
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int maxVotes = 0;
    // Compare the votes for all candidates, to find the largest number of votes
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > maxVotes)
        {
            maxVotes = candidates[i].votes;
        }
        // printf("votes for candidate %s: %i", candidates[i].name, candidates[i].votes);
    }
    // Print the candidiate(s) name with the largest number of votes
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == maxVotes)
        {
            printf("%s\n", candidates[i].name);
        }
    }
    return;
}

