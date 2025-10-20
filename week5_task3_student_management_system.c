// Week 5 – Task 3: Student Management System with text-file persistence
// Author: Nisa Yakut
// Build: gcc -Wall -Wextra -Werror -std=c11 -O2 week5_task3_student_management_system.c -o week5_task3_student_management_system
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 64

typedef struct {
    int    id;
    char   name[MAX_NAME];
    double gpa;
} Student;

typedef struct {
    Student *data;
    size_t   size;
    size_t   cap;
} StudentArray;

static void init_array(StudentArray *a) {
    a->data = NULL; a->size = 0; a->cap = 0;
}
static void free_array(StudentArray *a) {
    free(a->data); a->data = NULL; a->size = a->cap = 0;
}
static int ensure_capacity(StudentArray *a, size_t need) {
    if (need <= a->cap) return 0;
    size_t newcap = a->cap ? a->cap * 2 : 8;
    while (newcap < need) newcap *= 2;
    Student *tmp = (Student*)realloc(a->data, newcap * sizeof(Student));
    if (!tmp) return -1;
    a->data = tmp;
    a->cap = newcap;
    return 0;
}

static void trim_newline(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    if (n && s[n-1] == '\n') s[n-1] = '\0';
}

static int load_students(const char *fname, StudentArray *arr) {
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        // Graceful if file doesn't exist
        return 0;
    }
    char line[256];
    while (fgets(line, (int)sizeof line, fp) != NULL) {
        // Expected format: id;name;gpa
        int id; char name[MAX_NAME]; double gpa;
        // Use %[^\n] to read name with spaces until end-of-line or delimiter handling via sscanf with scanset.
        // We'll parse using semicolons as delimiters.
        char namebuf[MAX_NAME];
        int scanned = sscanf(line, "%d;%63[^;];%lf", &id, namebuf, &gpa);
        if (scanned == 3) {
            if (ensure_capacity(arr, arr->size + 1) != 0) { fclose(fp); return -1; }
            arr->data[arr->size].id = id;
            strncpy(arr->data[arr->size].name, namebuf, MAX_NAME);
            arr->data[arr->size].name[MAX_NAME-1] = '\0';
            arr->data[arr->size].gpa = gpa;
            arr->size++;
        }
        // silently skip malformed lines
    }
    if (ferror(fp)) {
        perror("read error");
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}

static int save_students(const char *fname, const StudentArray *arr) {
    FILE *fp = fopen(fname, "w");
    if (!fp) {
        perror("fopen for write");
        return -1;
    }
    for (size_t i = 0; i < arr->size; ++i) {
        const Student *s = &arr->data[i];
        // Persist as id;name;gpa (one record per line)
        if (fprintf(fp, "%d;%s;%.2f\n", s->id, s->name, s->gpa) < 0) {
            perror("fprintf");
            fclose(fp);
            return -1;
        }
    }
    fclose(fp);
    return 0;
}

static void list_students(const StudentArray *arr) {
    if (arr->size == 0) {
        printf("(No students loaded)\n");
        return;
    }
    printf("#  ID    Name                           GPA\n");
    printf("--------------------------------------------\n");
    for (size_t i = 0; i < arr->size; ++i) {
        printf("%-2zu %-5d %-30s %.2f\n",
               i+1, arr->data[i].id, arr->data[i].name, arr->data[i].gpa);
    }
}

static int add_student(StudentArray *arr) {
    Student s;
    char buf[256];

    printf("Enter name: ");
    if (!fgets(buf, (int)sizeof buf, stdin)) return -1;
    trim_newline(buf);
    if (buf[0] == '\0') { printf("Name cannot be empty.\n"); return 0; }
    strncpy(s.name, buf, MAX_NAME);
    s.name[MAX_NAME-1] = '\0';

    printf("Enter ID: ");
    if (!fgets(buf, (int)sizeof buf, stdin)) return -1;
    s.id = (int)strtol(buf, NULL, 10);

    printf("Enter GPA: ");
    if (!fgets(buf, (int)sizeof buf, stdin)) return -1;
    s.gpa = strtod(buf, NULL);

    if (ensure_capacity(arr, arr->size + 1) != 0) {
        fprintf(stderr, "Out of memory.\n");
        return -1;
    }
    arr->data[arr->size++] = s;
    printf("Student added successfully!\n");
    return 0;
}

static void menu_loop(const char *fname) {
    StudentArray arr; init_array(&arr);
    if (load_students(fname, &arr) != 0) {
        fprintf(stderr, "Failed to load students. Starting with empty list.\n");
    }

    for (;;) {
        printf("\n=== Student Management System ===\n");
        printf("1. List students\n");
        printf("2. Add student\n");
        printf("3. Save and Exit\n");
        printf("Select an option: ");
        char line[64];
        if (!fgets(line, (int)sizeof line, stdin)) break;
        int choice = (int)strtol(line, NULL, 10);
        if (choice == 1) {
            list_students(&arr);
        } else if (choice == 2) {
            if (add_student(&arr) != 0) {
                // continue even if minor error
            }
        } else if (choice == 3) {
            if (save_students(fname, &arr) == 0) {
                printf("Data saved. Goodbye!\n");
            }
            break;
        } else {
            printf("Unknown option.\n");
        }
    }
    free_array(&arr);
}

int main(void) {
    const char *fname = "students.txt";
    menu_loop(fname);
    return 0;
}