import cs50


def main():
    # Prompt user for text input to grade
    input_text = cs50.get_string("Text: ")

    letters = count_letters(input_text)
    words = count_words(input_text)
    sentences = count_sentences(input_text)
    L = (letters / words) * 100
    S = (sentences / words) * 100
    index = (0.0588 * L) - (0.296 * S) - 15.8
    grade = round(index)

    # Determine grade and print
    if grade < 1:
        print("Before Grade 1")
    elif grade > 16:
        print("Grade 16+")
    else:
        print(f"Grade: {grade}")


# Count the number of letters in the input string


def count_letters(text_string):
    count = 0
    for n in text_string:
        if n.isalnum() == True:
            count += 1
    return count

# Count the number of words in the input string


def count_words(text_string):
    count = 1
    for n in text_string:
        if n == " ":
            count += 1
    return count

# Count the number of sentences in the input string


def count_sentences(text_string):
    count = 0
    for n in text_string:
        if n == "." or n == "!" or n == "?":
            count += 1
    return count


main()