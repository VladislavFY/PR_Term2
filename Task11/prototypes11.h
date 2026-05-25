#include <stdio.h>
#include <math.h>

#define MAX_N 1000000000
#define METHOD_P 2

double f_value(double x);
double primitive_value(double x);
double exact_integral(double a, double b);
double rectangle_integral(double a, double b, int n);
int get_precision(double eps);
double runge_error(double current, double previous);
double calculate_integral(double a, double b, double eps, int *n, int *status);
double calculate_integral_runge(double a, double b, double eps, int *n, double *error, int *status);

double second_derivative_value(double x);
double get_m2_value(double a, double b);
int get_steps_by_error(double a, double b, double eps, int *status);
double calculate_integral_estimate(double a, double b, double eps, int *n, int *status);

int main_function(void);
