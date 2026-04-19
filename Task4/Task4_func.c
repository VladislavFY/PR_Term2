#include "prototypes4.h"

void free_matrix(int **array, int *sizes) {
    if (array != NULL) {
        free(array);
    }
    if (sizes != NULL) {
        free(sizes);
    }
}

int read_matrix(FILE *f, int ***array, int **sizes, int *rows) {
    int c;
    int in_number = 0;
    int count_in_row = 0;
    int total = 0;
    int capacity = 10;
    int current_row = 0;
    int i, j;

    if (f == NULL || array == NULL || sizes == NULL || rows == NULL) {
        return -1;
    }

    *rows = 0;
    *array = NULL;
    *sizes = NULL;

    *sizes = (int *)malloc(capacity * sizeof(int));
    if (*sizes == NULL) {
        return -2;
    }

    while ((c = fgetc(f)) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (in_number) {
                count_in_row++;
                total++;
                in_number = 0;
            }

            if (c == '\n') {
                if (count_in_row > 0) {
                    if (*rows >= capacity) {
                        capacity *= 2;
                        *sizes = (int *)realloc(*sizes, capacity * sizeof(int));
                        if (*sizes == NULL) {
                            return -2;
                        }
                    }
                    (*sizes)[*rows] = count_in_row;
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
        if (*rows >= capacity) {
            capacity *= 2;
            *sizes = (int *)realloc(*sizes, capacity * sizeof(int));
            if (*sizes == NULL) {
                return -2;
            }
        }
        (*sizes)[*rows] = count_in_row;
        (*rows)++;
    }

    if (*rows == 0) {
        free(*sizes);
        *sizes = NULL;
        return 0;
    }

    *array = (int **)malloc((*rows) * sizeof(int *) + total * sizeof(int));
    if (*array == NULL) {
        free(*sizes);
        *sizes = NULL;
        return -2;
    }

    (*array)[0] = (int *)(*array + *rows);
    for (i = 1; i < *rows; ++i) {
        (*array)[i] = (*array)[i - 1] + (*sizes)[i - 1];
    }

    rewind(f);

    for (i = 0; i < *rows; ++i) {
        for (j = 0; j < (*sizes)[i]; ++j) {
            if (fscanf(f, "%d", &((*array)[i][j])) != 1) {
                free(*array);
                free(*sizes);
                *array = NULL;
                *sizes = NULL;
                return -3;
            }
        }
    }

    return 0;
}

int write_matrix(FILE *f, int **array, int *sizes, int rows) {
    int i;
    int j;

    if (f == NULL) {
        return -1;
    }

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < sizes[i]; ++j) {
            fprintf(f, "%d", array[i][j]);
            if (j + 1 < sizes[i]) {
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

    if (array == NULL || sizes == NULL || rows < 0) {
        return -1;
    }

    for (i = 0; i < rows; ++i) {
        total += sizes[i];
    }

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

    if (array == NULL || sizes == NULL || rows <= 0) {
        return -1;
    }

    for (r = 0; r < rows; ++r) {
        ok = 1;

        for (c = 0; c < sizes[r]; ++c) {
            for (rr = 0; rr < rows; ++rr) {
                if (rr == r) {
                    continue;
                }

                if (sizes[rr] > c && array[rr][c] >= array[r][c]) {
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

    if (array == NULL || sizes == NULL || rows == NULL || *rows <= 0) {
        return;
    }

    len = sizes[del_row];

    for (i = 0; i < del_row; ++i) {
        start += sizes[i];
    }

    for (i = 0; i < *rows; ++i) {
        total += sizes[i];
    }

    for (z = 0; z < total; ++z) {
        if (z < start || z >= start + len) {
            array[0][q++] = array[0][z];
        }
    }

    for (i = del_row; i < *rows - 1; ++i) {
        sizes[i] = sizes[i + 1];
    }

    (*rows)--;

    if (*rows > 0) {
        base = array[0];
        array[0] = base;
        for (i = 1; i < *rows; ++i) {
            array[i] = array[i - 1] + sizes[i - 1];
        }
    }
}

int main_function(void) {
    FILE *f_in;
    FILE *f_out;
    int **array = NULL;
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
