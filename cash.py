import cs50


def main():
    quarter = 25
    dime = 10
    nickel = 5
    penny = 1
    owed = 0
    change = 0

    # Take user input for required change
    while owed < 0.01:
        owed = cs50.get_float("Change owed: ")

    # Convert owed to cents
    owed *= 100

    # Calculate how many quarters can be used
    while owed - quarter >= 0:
        owed -= quarter
        change += 1

    # Calculate how many dimes can be used
    while owed - dime >= 0:
        owed -= dime
        change += 1

    # Calculate how many nickels can be used
    while owed - nickel >= 0:
        owed -= nickel
        change += 1

    # Calculate how many pennies can be used
    while owed - penny >= 0:
        owed -= penny
        change += 1

    print(f"{change}")


main()