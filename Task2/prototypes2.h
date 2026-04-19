#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void free_matrix(int **array);
int read_matrix(FILE *f, int **array, int *rows, int *columns);
int write_matrix(FILE *f, int **array, int rows, int columns);
int check_matrix(int **array, int rows, int columns);
int **process_columns(int **array, int rows, int *columns, int M, int N);
int main_function(void);