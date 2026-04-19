#include "prototypes2.h"

int **process_columns(int **array, int rows, int *columns, int M, int N) {
    int original_col;
    int *ind;      
    int k = 0;     
    int del_count = 0;
    int q;
    int cur_row, cur_col;
    int need_delete;

    if (array == NULL || columns == NULL || *columns <= 0 || rows <= 0 || M <= 0) {
        return array;
    }

    original_col = *columns;

    ind = (int *)malloc(original_col * sizeof(int));
    if (ind == NULL) {
        return array;
    }

    for (int c = 0; c < original_col; ++c) {
        int ok = 1;  

        for (int r = 0; r < rows; ++r) {
            int rem = array[r][c] % M;
            if (rem < 0) {
                rem += M;
            }

            if (rem != N) {
                ok = 0;
                break;
            }
        }

        if (ok) {
            ind[k++] = c;
        }
    }

    if (k < 2) {
        free(ind);
        return array;
    }


    for (int t = 0; t + 1 < k; t += 2) {
        int c1 = ind[t];
        int c2 = ind[t + 1];

        for (int r = 0; r < rows; ++r) {
            if (array[r][c2] < array[r][c1]) {
                array[r][c1] = array[r][c2];
            }
        }

        del_count++; 
    }

    if (del_count == 0) {
        free(ind);
        return array;
    }

    *columns = original_col - del_count;

    q = 0;
    for (int z = 0; z < rows * original_col; ++z) {
        cur_row = z / original_col;
        cur_col = z % original_col;

        need_delete = 0;

        for (int t = 1; t < k; t += 2) {
            if (cur_col == ind[t]) {
                need_delete = 1;
                break;
            }
        }

        if (!need_delete) {
            array[0][q++] = array[0][z];
        }
    }


    for (int i = 1; i < rows; ++i) {
        array[i] = array[0] + i * (*columns);
    }

    free(ind);
    return array;
}

void free_matrix(int **array) {
    if (array == NULL) {
        return;
    }


    free(array);
}

int read_matrix(FILE *f, int **array, int *rows, int *columns) {
    int i;
    int j;

    if (f == NULL || array == NULL || rows == NULL || columns == NULL) {
        return -1;
    }

    for (i = 0; i < *rows; ++i) {
        for (j = 0; j < *columns; ++j) {
            if (fscanf(f, "%d", &(array[i][j])) != 1) {
                printf("Not enough numbers in the matrix!\n");
                return -4;
            }
        }
    }

    return 0;
}

int write_matrix(FILE *f, int **array, int rows, int columns) {
    int i;
    int j;

    if (f == NULL || array == NULL) {
        return -1;
    }

    fprintf(f, "%d %d\n", rows, columns);
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < columns; ++j) {
            fprintf(f, "%d", array[i][j]);
            if (j + 1 < columns) {
                fprintf(f, " ");
            }
        }
        fprintf(f, "\n");
    }

    return 0;
}

int check_matrix(int **array, int rows, int columns) {
    if (array == NULL || rows <= 0 || columns <= 0) {
        return -1;
    }

    for (int k = 0; k < rows * columns; ++k) {
        printf("%d ", array[0][k]);
    }
    printf("\n");

    return 0;
}

int main_function(void) {
    FILE *f_in;
    FILE *f_out;
    int **array;
    int rows = 0;
    int columns = 0;
    int M, N;
    int status;

    f_in = fopen("data.dat", "r");
    if (f_in == NULL) {
        return 1;
    }


    if (fscanf(f_in, "%d %d", &rows, &columns) != 2) {
        printf("Couldnt read rows and columns!\n");
        fclose(f_in);
        return -1;
    }

    if (fscanf(f_in, "%d %d", &M, &N) != 2) {
        printf("Couldnt read M and N!\n");
        fclose(f_in);
        return -1;
    }

    if (rows <= 0 || columns <= 0 || M <= 0 || N < 0 || N >= M) {
        printf("Invalid input for rows, columns, M or N!\n");
        fclose(f_in);
        return -2;
    }


    array = (int **)malloc(rows * sizeof(int *) + rows * columns * sizeof(int));
    if (array == NULL) {
        fclose(f_in);
        return 2;
    }

    array[0] = (int *)(array + rows);
    for (int i = 1; i < rows; ++i) {
        array[i] = array[i - 1] + columns;
    }

    status = read_matrix(f_in, array, &rows, &columns);
    fclose(f_in);

    if (status != 0) {
        free_matrix(array);
        return 2;
    }

    array = process_columns(array, rows, &columns, M, N);

    check_matrix(array, rows, columns);

    f_out = fopen("data.res", "w");
    if (f_out == NULL) {
        free_matrix(array);
        return 3;
    }

    write_matrix(f_out, array, rows, columns);
    fclose(f_out);

    free_matrix(array);
    return 0;
}
