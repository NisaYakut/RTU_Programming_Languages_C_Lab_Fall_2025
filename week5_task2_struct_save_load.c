// Week 5 – Task 2: Save & Load structured record (text format)
// Author: Nisa Yakut
// Build: gcc -Wall -Wextra -Werror -std=c11 -O2 week5_task2_struct_save_load.c -o week5_task2_struct_save_load
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char  name[64];
    int   age;
    double gpa;
} Student;

int save_student(const char *fname, const Student *s) {
    if (fname == NULL || s == NULL) return -1;
    FILE *fp = fopen(fname, "w");
    if (!fp) {
        perror("fopen for write");
        return -1;
    }
    // Simple space-separated text: name age gpa
    // Note: this format expects name without spaces.
    // For names with spaces, use quotes or another delimiter.
    if (fprintf(fp, "%s %d %.2f\n", s->name, s->age, s->gpa) < 0) {
        perror("fprintf");
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}

int load_student(const char *fname, Student *out) {
    if (fname == NULL || out == NULL) return -1;
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        perror("fopen for read");
        return -1;
    }
    // Read expected format
    char name[64];
    int age;
    double gpa;
    if (fscanf(fp, "%63s %d %lf", name, &age, &gpa) != 3) {
        fprintf(stderr, "Invalid file format.\n");
        fclose(fp);
        return -1;
    }
    fclose(fp);
    strncpy(out->name, name, sizeof(out->name));
    out->name[sizeof(out->name)-1] = '\0';
    out->age = age;
    out->gpa = gpa;
    return 0;
}

int main(void) {
    const char *fname = "student.txt";
    Student s = { "Alice", 21, 3.75 };
    printf("Saving student to file...\n");
    if (save_student(fname, &s) != 0) {
        return 1;
    }
    printf("Loading student from file...\n");
    Student loaded;
    if (load_student(fname, &loaded) != 0) {
        return 1;
    }
    printf("Loaded student: %s, %d, GPA %.2f\n", loaded.name, loaded.age, loaded.gpa);
    return 0;
}