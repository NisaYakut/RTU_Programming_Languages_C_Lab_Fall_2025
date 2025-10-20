#include <stdio.h>
#include <string.h>

struct Student {
  char name[50];
  int id;
  float grade;
};

static void print_student(const struct Student* s, int index) {
  printf("Student %d: %s, ID: %d, Grade: %.1f\n", index, s->name, s->id,
         s->grade);
}

int main(void) {
  struct Student s1, s2;

  strncpy(s1.name, "Alice Johnson", sizeof s1.name - 1);
  s1.name[sizeof s1.name - 1] = '\0';
  s1.id = 1001;
  s1.grade = 9.1f;

  strncpy(s2.name, "Bob Smith", sizeof s2.name - 1);
  s2.name[sizeof s2.name - 1] = '\0';
  s2.id = 1002;
  s2.grade = 8.7f;

  print_student(&s1, 1);
  print_student(&s2, 2);

  return 0;
}
