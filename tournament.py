# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []
    # Read teams into memory from file
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        for row in reader:
            item = row
            item["rating"] = int(item["rating"])
            teams.append(item)
    #for team in teams:
        #print(team)

    counts = {}
    # Simulate N tournaments and keep track of win counts
    # For N times, call simulate_tournament
    for n in range(N):
        winner = simulate_tournament(teams)
        # if "team" in counts True: increment counts for the returned team, else counts["team"] = x
        if winner not in counts:
            counts[winner] = 1
        else:
            counts[winner] += 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])
    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    # Call simulate_round while the teams list is > 1, then return the winning team
    winning_team = teams
    while len(winning_team) > 1:
        remaining_teams = simulate_round(winning_team)
        winning_team = remaining_teams
    #print(f"{winning_team[0]['team']}")
    return winning_team[0]['team']


if __name__ == "__main__":
    main()
