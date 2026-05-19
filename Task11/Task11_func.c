#include "prototypes11.h"

double f_value(double x) {
 double pi = acos(-1.0);
 return pow(pi, x) - exp(x);
}

double primitive_value(double x) {
 double pi = acos(-1.0);
 return pow(pi, x) / log(pi) - exp(x);
}

double exact_integral(double a, double b) {
 return primitive_value(b) - primitive_value(a);
}

double rectangle_integral(double a, double b, int n) {
 double h = (b - a) / n;
 double sum = 0.0;
 double x;
 int i;
 for (i = 0; i < n; ++i) {
  x = a + h * (i + 0.5);
  sum += f_value(x);
 }
 return sum * h;
}

int get_precision(double eps) {
 int precision = 0;
 while (eps < 1.0 && precision < 15) {
  eps *= 10.0;
  precision++;
 }
 return precision;
}

double runge_error(double current, double previous) {
 return fabs(current - previous) / (pow(2.0, METHOD_P) - 1.0);
}

double calculate_integral(double a, double b, double eps, int *n, int *status) {
 double previous;
 double current;
 *n = 1;
 previous = rectangle_integral(a, b, *n);
 while (*n <= MAX_N / 2) {
  *n *= 2;
  current = rectangle_integral(a, b, *n);
  if (fabs(current - previous) < eps) {
   *status = 0;
   return current;
  }
  previous = current;
 }
 *status = -1;
 return previous;
}

double calculate_integral_runge(double a, double b, double eps, int *n, double *error, int *status) {
 double previous;
 double current;
 *n = 1;
 previous = rectangle_integral(a, b, *n);
 while (*n <= MAX_N / 2) {
  *n *= 2;
  current = rectangle_integral(a, b, *n);
  *error = runge_error(current, previous);
  if (*error < eps) {
   *status = 0;
   return rectangle_integral(a, b, *n);
  }
  previous = current;
 }
 *status = -1;
 return previous;
}

int main_function(void) {
 double a;
 double b;
 double eps;
 double old_answer;
 double runge_answer;
 double exact_answer;
 double old_difference;
 double runge_difference;
 double methods_difference;
 double runge_method_error = 0.0;
 int old_n = 1;
 int runge_n = 1;
 int old_status = 0;
 int runge_status = 0;
 int precision;

 printf("Enter low integral value a: ");
 if (scanf("%lf", &a) != 1) {
  printf("Invalid a input!\n");
  return -1;
 }

 printf("Enter up integral value b: ");
 if (scanf("%lf", &b) != 1) {
  printf("Invalid b input!\n");
  return -1;
 }

 printf("Enter EPS(precision), for example 0.000001: ");
 if (scanf("%lf", &eps) != 1) {
  printf("Invalid EPS input!\n");
  return -1;
 }

 if (eps <= 0.0 || eps >= 1.0) {
  printf("EPS must satisfy 0 < eps < 1.\n");
  return -2;
 }

 precision = get_precision(eps);

 old_answer = calculate_integral(a, b, eps, &old_n, &old_status);
 runge_answer = calculate_integral_runge(a, b, eps, &runge_n, &runge_method_error, &runge_status);
 exact_answer = exact_integral(a, b);

 if (old_status == -1) {
  printf("Error: couldnt achieve needed accuracy by old method.\n");
  return -3;
 }

 if (runge_status == -1) {
  printf("Error: couldnt achieve needed accuracy by Runge method.\n");
  return -4;
 }

 old_difference = fabs(old_answer - exact_answer);
 runge_difference = fabs(runge_answer - exact_answer);
 methods_difference = fabs(old_answer - runge_answer);

 printf("Result from old rectangles method: %.*lf\n", precision, old_answer);
 printf("Result from Runge rectangles method: %.*lf\n", precision, runge_answer);
 printf("Result from Newton-Leibniz: %.*lf\n", precision, exact_answer);

 printf("Old method and Newton-Leibniz difference: %.*le\n", precision, old_difference);
 printf("Runge method and Newton-Leibniz difference: %.*le\n", precision, runge_difference);
 printf("Old method and Runge method difference: %.*le\n", precision, methods_difference);
 printf("Runge method error: %.*le\n", precision, runge_method_error);

 printf("Old method rectangles count: %d\n", old_n);
 printf("Runge method rectangles count: %d\n", runge_n);

 return 0;
}
