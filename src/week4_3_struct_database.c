#include <stdio.h>
#include <stdlib.h>

struct Student {
  char name[50];
  int id;
  float grade;
};

int main(void) {
  int n = 0;
  printf("Enter number of students: ");
  if (scanf("%d", &n) != 1 || n <= 0) {
    fprintf(stderr, "Invalid count.\n");
    return 1;
  }

  struct Student* arr = malloc((size_t)n * sizeof *arr);
  if (!arr) {
    fprintf(stderr, "Memory allocation failed.\n");
    return 1;
  }

  for (int i = 0; i < n; i++) {
    printf("Enter data for student %d (Name ID Grade): ", i + 1);
    if (scanf("%49s %d %f", arr[i].name, &arr[i].id, &arr[i].grade) != 3) {
      fprintf(stderr, "Invalid input for student %d.\n", i + 1);
      free(arr);
      return 1;
    }
  }

  printf("\n%-6s %-12s %-5s\n", "ID", "Name", "Grade");
  for (int i = 0; i < n; i++) {
    printf("%-6d %-12s %.1f\n", arr[i].id, arr[i].name, arr[i].grade);
  }

  free(arr);
  return 0;
}
