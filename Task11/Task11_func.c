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

int main_function(void) {
 double a;
 double b;
 double eps;
 double rectangle_answer;
 double exact_answer;
 double difference;
 int n = 1;
 int status = 0;
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

 rectangle_answer = calculate_integral(a, b, eps, &n, &status);
 exact_answer = exact_integral(a, b);
 difference = fabs(rectangle_answer - exact_answer);

 if (status == -1) {
  printf("Error: couldnt achieve needed accuracy.\n");
  return -3;
 }

 printf("Result from rectangles method: %.*lf\n", precision, rectangle_answer);
 printf("Result from Newton-Leibniz: %.*lf\n", precision, exact_answer);
 printf("Answers difference: %.*le\n", precision, difference);
 printf("Rectangles count: %d\n", n);

 return 0;
}
