#include <stdio.h>
#include <stdlib.h>

void free_matrix(int **array, int *sizes);
int read_matrix(FILE *f, int ***array, int **sizes, int *rows);
int write_matrix(FILE *f, int **array, int *sizes, int rows);
int check_matrix(int **array, int *sizes, int rows);
int find_row(int **array, int *sizes, int rows);
void delete_row(int **array, int *sizes, int *rows, int del_row);
int main_function(void);