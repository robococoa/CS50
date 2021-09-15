import cs50


def main():
    # Initialise height variable for user input
    height = 0
    row = 0
    # Set height to user input for the desired pyramid height
    while height < 1 or height > 8:
        height = cs50.get_int("Height: ")

    pyramid(height, row)

# Recursively print pyramid to console


def pyramid(height, row):
    if height > 1:
        pyramid(height - 1, row + 1)
    print(" " * row, end="")
    print("#" * height)


main()