#include "prototypes4.h"

void free_matrix(int **array, int *sizes) {
    if (array != NULL) {
        free(array);
    }
}

int read_matrix(FILE *f, int ***array, int **sizes, int *rows) {
    int c;
    int in_number = 0;
    int count_in_row = 0;
    int total = 0;
    int capacity = 10;
    int i, j;

    if (f == NULL || array == NULL || sizes == NULL || rows == NULL) {
        return -1;
    }

    *rows = 0;
    *array = NULL;

    while ((c = fgetc(f)) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (in_number) {
                count_in_row++;
                total++;
                in_number = 0;
            }

            if (c == '\n') {
                if (count_in_row > 0) {
                    (*rows)++;
                    count_in_row = 0;
                }
            }
        }
        else {
            in_number = 1;
        }
    }

    if (in_number) {
        count_in_row++;
        total++;
    }

    if (count_in_row > 0) {
        (*rows)++;
    }

    if (*rows == 0) {
        return 0;
    }

    while (capacity < *rows + 1) {
        capacity *= 2;
    }

    *array = (int **)malloc(capacity * sizeof(int *) + total * sizeof(int));
    if (*array == NULL) {
        return -2;
    }

    (*array)[0] = (int *)(*array + capacity);

    rewind(f);

    i = 0;
    j = 0;

    /* Изменение: читаем числа и одновременно строим указатели строк,
       поэтому отдельный массив sizes больше не нужен. */
    while ((c = fgetc(f)) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (in_number) {
                if (fseek(f, -1L, SEEK_CUR) != 0) {
                    free(*array);
                    *array = NULL;
                    return -3;
                }

                if (fscanf(f, "%d", &((*array)[0][j])) != 1) {
                    free(*array);
                    *array = NULL;
                    return -3;
                }

                j++;
                in_number = 0;
            }

            if (c == '\n') {
                if (i < *rows && (i == 0 || (*array)[i] != (*array)[i - 1])) {
                    i++;
                    (*array)[i] = (*array)[0] + j;
                }
            }
        }
        else {
            in_number = 1;
        }
    }

    if (in_number) {
        if (fseek(f, -1L, SEEK_CUR) != 0) {
            free(*array);
            *array = NULL;
            return -3;
        }

        if (fscanf(f, "%d", &((*array)[0][j])) != 1) {
            free(*array);
            *array = NULL;
            return -3;
        }

        j++;
    }

    /* Изменение: последний указатель показывает конец последней строки. */
    (*array)[*rows] = (*array)[0] + total;

    return 0;
}

int write_matrix(FILE *f, int **array, int *sizes, int rows) {
    int i;
    int j;
    int len;

    if (f == NULL) {
        return -1;
    }

    /* Изменение: sizes больше не используется. */
    (void)sizes;

    for (i = 0; i < rows; ++i) {
        /* Изменение: длина строки считается разностью соседних указателей. */
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

int check_matrix(int **array, int *sizes, int rows) {
    int i;
    int total = 0;

    if (array == NULL || rows < 0) {
        return -1;
    }

    /* Изменение: sizes больше не используется. */
    (void)sizes;

    /* Изменение: всего элементов = конец последней строки - начало первой строки. */
    total = array[rows] - array[0];

    for (i = 0; i < total; ++i) {
        printf("%d ", array[0][i]);
    }
    printf("\n");

    return 0;
}

int find_row(int **array, int *sizes, int rows) {
    int r;
    int c;
    int rr;
    int ok;
    int len_r;
    int len_rr;

    if (array == NULL || rows <= 0) {
        return -1;
    }

    /* Изменение: sizes больше не используется. */
    (void)sizes;

    for (r = 0; r < rows; ++r) {
        ok = 1;

        /* Изменение: длина строки берется через разность указателей. */
        len_r = array[r + 1] - array[r];

        for (c = 0; c < len_r; ++c) {
            for (rr = 0; rr < rows; ++rr) {
                if (rr == r) {
                    continue;
                }

                /* Изменение: длина другой строки тоже берется через разность указателей. */
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

void delete_row(int **array, int *sizes, int *rows, int del_row) {
    int start = 0;
    int len;
    int total = 0;
    int z;
    int q = 0;
    int i;
    int *base;

    if (array == NULL || rows == NULL || *rows <= 0) {
        return;
    }

    /* Изменение: sizes больше не используется. */
    (void)sizes;

    /* Изменение: начало удаляемой строки считаем через адрес. */
    start = array[del_row] - array[0];

    /* Изменение: длина удаляемой строки считается разностью соседних указателей. */
    len = array[del_row + 1] - array[del_row];

    /* Изменение: общее количество элементов тоже считается через указатели. */
    total = array[*rows] - array[0];

    for (z = 0; z < total; ++z) {
        if (z < start || z >= start + len) {
            array[0][q++] = array[0][z];
        }
    }

    /* Изменение: после удаления строки сдвигаем сами указатели строк. */
    for (i = del_row; i < *rows; ++i) {
        array[i] = array[i + 1] - len;
    }

    (*rows)--;

    if (*rows > 0) {
        base = array[0];
        array[0] = base;
    }
}

int main_function(void) {
    FILE *f_in;
    FILE *f_out;
    int **array = NULL;

    /* Изменение: sizes оставлен только чтобы меньше менять вызовы функций. */
    int *sizes = NULL;

    int rows = 0;
    int status;
    int del_row;

    f_in = fopen("data.dat", "r");
    if (f_in == NULL) {
        return 1;
    }

    status = read_matrix(f_in, &array, &sizes, &rows);
    fclose(f_in);

    if (status != 0) {
        free_matrix(array, sizes);
        return 2;
    }

    del_row = find_row(array, sizes, rows);
    if (del_row != -1) {
        delete_row(array, sizes, &rows, del_row);
    }

    check_matrix(array, sizes, rows);

    f_out = fopen("data.res", "w");
    if (f_out == NULL) {
        free_matrix(array, sizes);
        return 3;
    }

    write_matrix(f_out, array, sizes, rows);
    fclose(f_out);

    free_matrix(array, sizes);
    return 0;
}
