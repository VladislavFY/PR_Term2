def read_numbers(filename):
    with open(filename, "r", encoding="utf-8") as file:
        for line in file:
            for token in line.split():
                yield float(token)


def check_sequence_type(filename):
    is_increasing = True
    is_decreasing = True
    prev = None
    count = 0

    for current in read_numbers(filename):
        if prev is not None:
            if current <= prev:
                is_increasing = False
            if current >= prev:
                is_decreasing = False

        prev = current
        count += 1

    return count, is_increasing, is_decreasing


def main():
    filename = input("Введите имя файла: ")

    try:
        count, is_increasing, is_decreasing = check_sequence_type(filename)
    except FileNotFoundError:
        print("Файл не удалось открыть.")
        return

    if count == 0:
        print("Последовательность пустая.")
    else:
        print("Возрастающая:", "да" if is_increasing else "нет")
        print("Убывающая:", "да" if is_decreasing else "нет")


if __name__ == "__main__":
    main()
