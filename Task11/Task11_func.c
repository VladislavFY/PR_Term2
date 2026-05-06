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

 printf("Введите нижний предел a: ");
 if (scanf("%lf", &a) != 1) {
  printf("Ошибка ввода a!\n");
  return -1;
 }

 printf("Введите верхний предел b: ");
 if (scanf("%lf", &b) != 1) {
  printf("Ошибка ввода b!\n");
  return -1;
 }

 printf("Введите точность eps, например 0.000001: ");
 if (scanf("%lf", &eps) != 1) {
  printf("Ошибка ввода eps!\n");
  return -1;
 }

 if (eps <= 0.0 || eps >= 1.0) {
  printf("Точность должна быть в промежутке 0 < eps < 1.\n");
  return -2;
 }

 precision = get_precision(eps);

 rectangle_answer = calculate_integral(a, b, eps, &n, &status);
 exact_answer = exact_integral(a, b);
 difference = fabs(rectangle_answer - exact_answer);

 if (status == -1) {
  printf("Ошибка: не удалось достичь нужной точности.\n");
  return -3;
 }

 printf("Интеграл методом прямоугольников: %.*lf\n", precision, rectangle_answer);
 printf("Интеграл по формуле Ньютона-Лейбница: %.*lf\n", precision, exact_answer);
 printf("Разница между ответами: %.*le\n", precision, difference);
 printf("Количество прямоугольников: %d\n", n);

 return 0;
}
