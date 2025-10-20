#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int n = 0;
  printf("Enter number of elements: ");
  if (scanf("%d", &n) != 1 || n <= 0) {
    fprintf(stderr, "Invalid n.\n");
    return 1;
  }

  int* a = malloc((size_t)n * sizeof *a);
  if (!a) {
    fprintf(stderr, "Memory allocation failed.\n");
    return 1;
  }

  printf("Enter %d integers: ", n);
  long long sum = 0;
  for (int i = 0; i < n; i++) {
    if (scanf("%d", &a[i]) != 1) {
      fprintf(stderr, "Invalid input at index %d.\n", i);
      free(a);
      return 1;
    }
    sum += a[i];
  }

  double avg = (double)sum / (double)n;
  printf("Sum = %lld\n", sum);
  printf("Average = %.2f\n", avg);

  free(a);
  return 0;
}
