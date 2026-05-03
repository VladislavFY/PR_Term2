double **mem_st(double **a, int max_N, int act_N, int elem_in_row, int size)
{
    double **b;
    int j;
    int used_elements;

    b = (double **) malloc(size);
    if (b == NULL) return NULL; 

    /*
        b1 = a[0];
        b[0] = (double *)(b + max_N);

        for (j = 1; j < act_N; j++)
        {
            b2 = a[j];
            b[j] = b[j-1] + (b2 - b1);
            b1 = b2;
        }

        Problem:
        This does not rebuild all needed row pointers safely.
        You need pointers up to b[act_N], because b[act_N]
        points to the start of the current row / end of previous row.
    */

    b[0] = (double *)(b + max_N);

    /*
        Count how many double values must be copied.

        a[act_N] - a[0] gives the number of elements
        before the current row.

        elem_in_row gives the number of elements already read
        in the current row.
    */
    used_elements = (int)(a[act_N] - a[0]) + elem_in_row;

    /*
        if (act_N != 0) 
        {
            b[act_N] = (double *)(b[act_N - 1] + elem_in_row);
            for (j = 0; j < (b[act_N] - b[0]); j++)
                b[0][j] = a[0][j];
        }
        else
        {	
            for (j = 0; j < elem_in_row; j++)
                b[0][j] = a[0][j];
        }

        Problem:
        b[act_N - 1] may not yet be correctly initialized,
        and the amount of data copied can be wrong.
    */

    for (j = 0; j < used_elements; j++) {
        b[0][j] = a[0][j];
    }
    for (j = 1; j <= act_N; j++) {
        b[j] = b[j - 1] + (a[j] - a[j - 1]);
    }

    free(a);

    return b;
}

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
