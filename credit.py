import cs50
import sys


def main():
    # Prompt user for card number
    card_number = cs50.get_string("Number: ")
    length = len(card_number)

    # Check for card type
    # VISA
    if (length == 13 or length == 16) and card_number.startswith("4"):
        if checksum(card_number):
            print("VISA")
    # Mastercard
    elif length == 16 and (card_number.startswith("51") or card_number.startswith("52") or card_number.startswith("53") or card_number.startswith("54") or card_number.startswith("55")):
        if checksum(card_number):
            print("MASTERCARD")
    # American Express
    elif length == 15 and (card_number.startswith("34") or card_number.startswith("37")):
        if checksum(card_number):
            print("AMEX")
    else:
        print("INVALID")

# Run Checksum over number


def checksum(card_number):
    card = int(card_number)
    sum_of_digits = 0
    sum_of_others = 0
    multiplied = 0
    count = 1

    # Check each digit starting from the end
    while card > 10:
        digit = int(card % 10)
        card = int(card / 10)
        # Each end digit, starting from last
        if count % 2 == 0:
            multiplied = digit * 2
            # Separate the digits out if there's more than 1, before adding to the sum
            if int(multiplied / 10) > 0:
                multiplied_digits = multiplied
                while multiplied_digits > 9:
                    sum_of_digits += multiplied % 10
                    multiplied_digits = int(multiplied_digits / 10)
                sum_of_digits += multiplied_digits
            # If there is only 1 digit, add it to the sum
            else:
                sum_of_digits += multiplied
        # Each 2nd digit starting from 2nd last
        else:
            sum_of_others += digit
        count += 1
    # If there's 1 digit left, add to appropriate sum, including the multiply step
    card = int(card)
    if count % 2 == 0:
        # If the digit is a 5, this will become 10 when x2, so the sum will be 1. Cannot be 6+ due to valid card prefixes
        if card == 5:
            sum_of_digits += 1
        else:
            sum_of_digits += card * 2
    else:
        sum_of_others += int(card)
    # Add the remaining digits from the card number to the summation
    sum_of_digits += sum_of_others

    # If the last digit of the final sum is zero, it passes the checksum
    if sum_of_digits % 10 == 0:
        return True
    else:
        print("INVALID")
        return False


main()
