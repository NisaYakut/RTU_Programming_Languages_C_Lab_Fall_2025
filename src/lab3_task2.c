#include <stdio.h>

// Name: Nisa Yakut
// Student ID: (buraya yaz)

// Function to swap two integers
void swap(int* x, int* y) {
  if (x == NULL || y == NULL) return;
  int temp = *x;
  *x = *y;
  *y = temp;
}

// Function to modify value (multiply by 2)
void modify_value(int* x) {
  if (x == NULL) return;
  *x = (*x) * 2;
}

int main(void) {
  int a = 3, b = 7;

  printf("Before swap: a=%d, b=%d\n", a, b);
  swap(&a, &b);
  printf("After swap: a=%d, b=%d\n", a, b);

  modify_value(&a);
  printf("After modify_value: a=%d\n", a);

  return 0;
}
