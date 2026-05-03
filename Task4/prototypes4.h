
#include <stdio.h>
#include <stdlib.h>

int **mem_st(int **a, int max_N, int act_N, int elem_in_row, int size);
void free_matrix(int **array);
int read_matrix(FILE *f, int ***array, int *rows);
int write_matrix(FILE *f, int **array, int rows);
int check_matrix(int **array, int rows);
int find_row(int **array, int rows);
void delete_row(int **array, int *rows, int del_row);
int main_function(void);
