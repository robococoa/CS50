import cs50
import csv
import sys


def main():
    # Check for the correct number of command-line arguments
    if len(sys.argv) != 3:
        print("Usage: python dna.py database.csv sequence.txt")
        sys.exit(1)

    # Read csv into memory
    database = []
    with open(sys.argv[1]) as csv_file:
        reader = csv.DictReader(csv_file)
        for row in reader:
            database.append(row)

    # Read sequence.txt into memory
    sequence_file = open(sys.argv[2], "r")
    # Convert sequence file into string
    sequence = sequence_file.read()
    length = len(sequence)

    # Populate counts with STR's from database
    counts = {}
    for key in database[0].keys():
        if key != "name":
            counts[key] = 0

    # Search sequence for longest consecutive matching STR's
    # Start with a STR from the counts dict
    for STR in counts:
        test_length = len(STR)
        # Search through the length of the sequence for matches
        check_for_match(STR, sequence, length, counts)
        for test in range(0, (length + 1), test_length):
            # Check if STR matches in sequence
            tmp = sequence[test:test + test_length]
    # Check for matching person, must match all values in counts{}
    required_matches = len(counts)
    # Go through each count STR and then find that same STR in the database to compare values
    for person in database:
        matches = 0
        for count in counts:
            if int(person[count]) == counts[count]:
                matches += 1
                print(person, matches)
                if matches == required_matches:
                    print(person["name"])
                    return
    # If no person had an exact match across all STR values, end with no match found
    print("No match")
    return


# Compares two strings for a match
def check_for_match(STR, dna_sequence, sequence_length, counts):
    # Keep track of the number of consecutive matches
    matches = 0
    last = 0
    i = 0
    # Search each letter for a match to the first letter of STR
    while i < sequence_length:
        # If first letter matches, check if whole STR matches
        if STR[0] == dna_sequence[i]:
            if dna_sequence[i:(i + len(STR))] == STR:
                # Check if this match is continuing from another match, or is the start of a new matching sequence
                if last == 0 or i - len(STR) == last:
                    matches += 1
                    last = i
                    i += len(STR) - 1
                # This is a new match sequence, reset counts to be now the 1st match. Check if the current counts should be saved into counts
                else:
                    if counts[STR] < matches:
                        counts[STR] = matches
                    matches = 1
                    last = 0
            else:
                if counts[STR] < matches:
                    counts[STR] = matches
                matches = 0
                last = 0
        i += 1
    # Save any orphaned match sequence
    if counts[STR] < matches:
        counts[STR] = matches
    return


main()