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
        #print(count[0:len(count)] + f" is length: {test_length}")                                                                              # debugging
        # Search through the length of the sequence for matches
        check_for_match(STR, sequence, length, counts)
        for test in range(0, (length + 1), test_length):
            # Check if STR matches in sequence
            tmp = sequence[test:test + test_length]
    # Check for matching person
    # Go through each count STR
    for count in counts:
        for person in database:
            if int(person[count]) == counts[count]:
                print(person["name"])
                return
                #print(f"for {count}, person[count]: {person[count]}, counts[count]: {counts[count]}")
    print("No match")
    return


# Compares two strings for a match
def check_for_match(STR, dna_sequence, sequence_length, counts):
    # Keep track of the number of consecutive matches
    matches = 0
    last = 0
    i = 0
    # Search each letter for a match to the first letter of STR
    #for i in range(0, sequence_length):
    while i < sequence_length:
        #print(f"for {i}, compare {STR}:{STR[0]} with {dna_sequence[i]}")                                                                              # debugging
        # If first letter matches, check if whole STR matches
        if STR[0] == dna_sequence[i]:
            #print(f"does {STR} match dna_sequence, {dna_sequence[i:i + len(STR)]}")                                                                              # debugging
            if dna_sequence[i:(i + len(STR))] == STR:
                #print("matched")
                # Check if this match is continuing from another match, or is the start of a new matching sequence
                if last == 0 or i - len(STR) == last:
                    #print(f"{STR} matches {dna_sequence[i:i + len(STR)]}")                                                                              # debugging
                    matches += 1
                    last = i
                    #print(f"last is now: {last}, i is {i}, matches is {matches}, next {i} letters are {dna_sequence[i:i + len(STR)]}")
                    i += len(STR) - 1
                    #print(f"i is now: {i}, increased by {len(STR)}")
                # This is a new match sequence, reset counts to be now the 1st match. Check if the current counts should be saved into counts
                else:
                    if counts[STR] < matches:
                        counts[STR] = matches
                    matches = 1
                    last = 0
                    #print(f"last is reset to current i: {last}, matches is {matches}")
                    #print(f"i is reset to current: {i}")
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