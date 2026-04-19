def read_numbers(filename):
    with open(filename, "r", encoding="utf-8") as file:
        for line in file:
            for token in line.split():
                yield float(token)


def mean_square_deviation(filename):
    n = 0
    sum_x = 0.0
    sum_x2 = 0.0

    for x in read_numbers(filename):
        n += 1
        sum_x += x
        sum_x2 += x * x

    if n == 0:
        return None

    mean = sum_x / n
    d = sum_x2 / n - mean * mean

    return d


def main():
    filename = input("Введите имя файла: ")

    try:
        result = mean_square_deviation(filename)
    except FileNotFoundError:
        print("Файл не удалось открыть.")
        return

    if result is None:
        print("Последовательность пустая.")
    else:
        print("Среднее квадратическое отклонение от среднего арифметического:", result)


if __name__ == "__main__":
    main()