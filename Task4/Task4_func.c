#include "prototypes4.h"

int **mem_st(int **a, int max_N, int act_N, int elem_in_row, int size)
{
    int **b;
    int j;
    int used_elements;

    b = (int **)malloc(size);
    if (b == NULL) {
        return NULL;
    }

    b[0] = (int *)(b + max_N);

    if (a != NULL) {
        used_elements = (int)(a[act_N] - a[0]) + elem_in_row;

        for (j = 0; j < used_elements; ++j) {
            b[0][j] = a[0][j];
        }

        for (j = 1; j <= act_N; ++j) {
            b[j] = b[j - 1] + (a[j] - a[j - 1]);
        }

        free(a);
    }

    return b;
}

void free_matrix(int**array) {
    if (array != NULL) {
        free(array);
    }
}

int read_matrix(FILE *f, int ***array, int *rows) {
    int c;
    int value;
    int elem_in_row = 0;
    int max_N = 10;
    int max_M = 10;
    int size;
    int **tmp;

    if (f == NULL || array == NULL || rows == NULL) {
        return -1;
    }

    *rows = 0;
    *array = NULL;

    size = max_N * sizeof(int *) + max_M * sizeof(int);

    /* Сразу выделяем один общий блок памяти: сначала указатели строк,
       потом все элементы матрицы подряд, как в mem_st. */
    *array = mem_st(NULL, max_N, *rows, elem_in_row, size);
    if (*array == NULL) {
        return -2;
    }

    while ((c = fgetc(f)) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (c == '\n' && elem_in_row > 0) {
                /* Перед добавлением новой границы строки проверяем,
                   хватает ли места под указатели строк. */
                if (*rows + 1 >= max_N) {
                    max_N *= 2;
                    size = max_N * sizeof(int *) + max_M * sizeof(int);

                    tmp = mem_st(*array, max_N, *rows, elem_in_row, size);
                    if (tmp == NULL) {
                        free(*array);
                        *array = NULL;
                        return -2;
                    }

                    *array = tmp;
                }

                /* Конец текущей строки становится началом следующей.
                   Поэтому длина строки теперь хранится не отдельно,
                   а как разность array[i + 1] - array[i]. */
                (*array)[*rows + 1] = (*array)[*rows] + elem_in_row;
                (*rows)++;
                elem_in_row = 0;
            }
        }
        else {
            ungetc(c, f);

            if (fscanf(f, "%d", &value) != 1) {
                free(*array);
                *array = NULL;
                return -3;
            }

            /* Проверяем, хватает ли места под очередное число. */
            if ((*array)[*rows] + elem_in_row - (*array)[0] >= max_M) {
                max_M *= 2;
                size = max_N * sizeof(int *) + max_M * sizeof(int);

                tmp = mem_st(*array, max_N, *rows, elem_in_row, size);
                if (tmp == NULL) {
                    free(*array);
                    *array = NULL;
                    return -2;
                }

                *array = tmp;
            }

            (*array)[*rows][elem_in_row] = value;
            elem_in_row++;
        }
    }

    if (elem_in_row > 0) {
        if (*rows + 1 >= max_N) {
            max_N *= 2;
            size = max_N * sizeof(int *) + max_M * sizeof(int);

            tmp = mem_st(*array, max_N, *rows, elem_in_row, size);
            if (tmp == NULL) {
                free(*array);
                *array = NULL;
                return -2;
            }

            *array = tmp;
        }

        /* Добавляем последнюю границу, чтобы знать конец последней строки. */
        (*array)[*rows + 1] = (*array)[*rows] + elem_in_row;
        (*rows)++;
    }

    if (*rows == 0) {
        free(*array);
        *array = NULL;
    }

    return 0;
}

int write_matrix(FILE *f, int **array, int rows) {
    int i;
    int j;
    int len;

    if (f == NULL) {
        return -1;
    }

    if (array == NULL && rows > 0) {
        return -1;
    }

    for (i = 0; i < rows; ++i) {
        /* Длину строки считаем через соседние указатели. */
        len = array[i + 1] - array[i];

        for (j = 0; j < len; ++j) {
            fprintf(f, "%d", array[i][j]);
            if (j + 1 < len) {
                fprintf(f, " ");
            }
        }

        if (i + 1 < rows) {
            fprintf(f, "\n");
        }
    }

    return 0;
}

int check_matrix(int **array, int rows) {
    int i;
    int total = 0;

    if (array == NULL || rows < 0) {
        return -1;
    }

    /* Общее количество элементов — это конец последней строки минус начало данных. */
    total = array[rows] - array[0];

    for (i = 0; i < total; ++i) {
        printf("%d ", array[0][i]);
    }
    printf("\n");

    return 0;
}

int find_row(int **array, int rows) {
    int r;
    int c;
    int rr;
    int ok;
    int len_r;
    int len_rr;

    if (array == NULL || rows <= 0) {
        return -1;
    }

    for (r = 0; r < rows; ++r) {
        ok = 1;

        /* Длина проверяемой строки хранится через разность указателей. */
        len_r = array[r + 1] - array[r];

        for (c = 0; c < len_r; ++c) {
            for (rr = 0; rr < rows; ++rr) {
                if (rr == r) {
                    continue;
                }

                /* Длина другой строки тоже считается через указатели. */
                len_rr = array[rr + 1] - array[rr];

                if (len_rr > c && array[rr][c] >= array[r][c]) {
                    ok = 0;
                    break;
                }
            }

            if (!ok) {
                break;
            }
        }

        if (ok) {
            return r;
        }
    }

    return -1;
}

void delete_row(int **array, int *rows, int del_row) {
    int start;
    int len;
    int total;
    int z;
    int q = 0;
    int i;

    if (array == NULL || rows == NULL || *rows <= 0) {
        return;
    }

    if (del_row < 0 || del_row >= *rows) {
        return;
    }

    /* Начало удаляемой строки — это смещение ее указателя от начала данных. */
    start = array[del_row] - array[0];

    /* Длина удаляемой строки — разность соседних указателей. */
    len = array[del_row + 1] - array[del_row];

    /* Всего элементов — от начала данных до конца последней строки. */
    total = array[*rows] - array[0];

    for (z = 0; z < total; ++z) {
        if (z < start || z >= start + len) {
            array[0][q++] = array[0][z];
        }
    }

    /* После удаления строки сдвигаем указатели строк.
       Все строки после удаленной теперь начинаются на len элементов раньше. */
    for (i = del_row; i < *rows; ++i) {
        array[i] = array[i + 1] - len;
    }

    (*rows)--;
}

int main_function(void) {
    FILE *f_in;
    FILE *f_out;
    int **array = NULL;
    int rows = 0;
    int status;
    int del_row;

    f_in = fopen("data.dat", "r");
    if (f_in == NULL) {
        return 1;
    }

    status = read_matrix(f_in, &array, &rows);
    fclose(f_in);

    if (status != 0) {
        free_matrix(array);
        return 2;
    }

    del_row = find_row(array, rows);
    if (del_row != -1) {
        delete_row(array, &rows, del_row);
    }

    check_matrix(array, rows);

    f_out = fopen("data.res", "w");
    if (f_out == NULL) {
        free_matrix(array);
        return 3;
    }

    write_matrix(f_out, array, rows);
    fclose(f_out);

    free_matrix(array);
    return 0;
}
