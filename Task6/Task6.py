def read_numbers(filename):
    with open(filename, "r", encoding="utf-8") as file:
        for line in file:
            for token in line.split():
                yield float(token)


def check_recurrence(filename, c1, c2, c3, b, eps=1e-9):
    iterator = read_numbers(filename)

    try:
        prev2 = next(iterator)
        prev1 = next(iterator)
    except StopIteration:
        return True

    for current in iterator:
        left_side = c1 * current + c2 * prev1 + c3 * prev2

        if abs(left_side - b) > eps:
            return False

        prev2 = prev1
        prev1 = current

    return True


def main():
    filename = input("Введите имя файла: ")
    c1 = float(input("Введите c1: "))
    c2 = float(input("Введите c2: "))
    c3 = float(input("Введите c3: "))
    b = float(input("Введите b: "))

    try:
        result = check_recurrence(filename, c1, c2, c3, b)
    except FileNotFoundError:
        print("Файл не удалось открыть.")
        return

    if result:
        print("Последовательность удовлетворяет рекуррентному соотношению.")
    else:
        print("Последовательность не удовлетворяет рекуррентному соотношению.")


if __name__ == "__main__":
    main()