#include <stdio.h>

// Name: Nisa Yakut
// Student ID: (buraya yaz)

int array_min(int arr[], int size) {
  if (size <= 0) return 0;
  int min = arr[0];
  for (int i = 1; i < size; i++) {
    if (arr[i] < min) min = arr[i];
  }
  return min;
}

int array_max(int arr[], int size) {
  if (size <= 0) return 0;
  int max = arr[0];
  for (int i = 1; i < size; i++) {
    if (arr[i] > max) max = arr[i];
  }
  return max;
}

int array_sum(int arr[], int size) {
  int sum = 0;
  for (int i = 0; i < size; i++) sum += arr[i];
  return sum;
}

float array_avg(int arr[], int size) {
  if (size <= 0) return 0.0f;
  return (float)array_sum(arr, size) / size;
}

int main(void) {
  int arr[] = {10, 20, 5, 30, 15};
  int size = 5;

  printf("Min: %d\n", array_min(arr, size));
  printf("Max: %d\n", array_max(arr, size));
  printf("Sum: %d\n", array_sum(arr, size));
  printf("Avg: %.2f\n", array_avg(arr, size));

  return 0;
}
