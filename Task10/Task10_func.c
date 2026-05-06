#include "prototypes10.h"

double f_value(double x) {
 return log(x + 3.0) / log(2.0) - log(x + 5.0) / log(3.0);
}

double f_derivative(double x) {
 return 1.0 / ((x + 3.0) * log(2.0)) -
        1.0 / ((x + 5.0) * log(3.0));
}

int get_precision(double eps) {
 int precision = 0;

 while (eps < 1.0 && precision < 15) {
  eps *= 10.0;
  precision++;
 }

 return precision;
}

double newton_method(double x0, double eps, int *iter, int *status) {
 double x1;
 double step;
 double lambda;
 int i = 0;

 if (x0 <= -3.0) {
  *status = -1;
  return 0.0;
 }

 while (i < MAX_ITER) {
  if (fabs(f_derivative(x0)) < 1e-12) {
   *status = -2;
   return 0.0;
  }

  step = f_value(x0) / f_derivative(x0);
  lambda = 1.0;

  x1 = x0 - lambda * step;

  while (x1 <= -3.0) {
   lambda /= 2.0;
   x1 = x0 - lambda * step;

   if (lambda < 1e-12) {
    *status = -3;
    return 0.0;
   }
  }

  i++;

  if (fabs(x1 - x0) < eps) {
   *iter = i;
   *status = 0;
   return x1;
  }

  x0 = x1;
 }

 *iter = i;
 *status = -4;
 return x0;
}

int main_function(void) {
 double x0;
 double eps;
 double answer;
 int iter = 0;
 int status = 0;
 int precision;

 printf("Enter initial x0 value, x0 > -3: ");
 if (scanf("%lf", &x0) != 1) {
  printf("Invalid x0 input!\n");
  return -1;
 }

 printf("Enter EPS value, for example 0.000001: ");
 if (scanf("%lf", &eps) != 1) {
  printf("Invalid EPS input!\n");
  return -1;
 }

 if (eps <= 0.0 || eps >= 1.0) {
  printf("EPS must satisfy inequation 0 < eps < 1.\n");
  return -2;
 }

 precision = get_precision(eps);
 answer = newton_method(x0, eps, &iter, &status);

 if (status == -1) {
  printf("Error: x0 value must be bigger than -3.\n");
  return -3;
 }

 if (status == -2) {
  printf("Error: deriviative to close to zero.\n");
  return -4;
 }

 if (status == -3) {
  printf("Error: escaped the domain of definition.\n");
  return -5;
 }

 if (status == -4) {
  printf("Error: done more iterations than possible.\n");
  return -6;
 }

 printf("Root of the equation: x = %.*lf\n", precision, answer);
 printf("Value of function in root point: f(x) = %.*le\n", precision, f_value(answer));
 printf("Iteration count: %d\n", iter);

 return 0;
}
