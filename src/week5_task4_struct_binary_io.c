// Week 5 – Task 4 (Optional): Binary file I/O for Student
// Author: Nisa Yakut
// Build: gcc -Wall -Wextra -Werror -std=c11 -O2 week5_task4_struct_binary_io.c -o week5_task4_struct_binary_io
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char  name[64];
    int   age;
    double gpa;
} Student;

static int save_student_bin(const char *fname, const Student *s) {
    FILE *fp = fopen(fname, "wb");
    if (!fp) { perror("fopen wb"); return -1; }
    size_t n = fwrite(s, sizeof *s, 1, fp);
    if (n != 1) { perror("fwrite"); fclose(fp); return -1; }
    fclose(fp);
    return 0;
}

static int load_student_bin(const char *fname, Student *out) {
    FILE *fp = fopen(fname, "rb");
    if (!fp) { perror("fopen rb"); return -1; }
    size_t n = fread(out, sizeof *out, 1, fp);
    if (n != 1) { fprintf(stderr, "fread failed or file too short.\n"); fclose(fp); return -1; }
    fclose(fp);
    return 0;
}

static long file_size(const char *fname) {
    FILE *fp = fopen(fname, "rb");
    if (!fp) return -1;
    if (fseek(fp, 0, SEEK_END) != 0) { fclose(fp); return -1; }
    long sz = ftell(fp);
    fclose(fp);
    return sz;
}

int main(void) {
    const char *binname = "student.bin";
    Student s = { "Alice", 21, 3.75 };
    printf("Saving student in binary format...\n");
    if (save_student_bin(binname, &s) != 0) return 1;

    printf("Loading student from binary file...\n");
    Student t;
    if (load_student_bin(binname, &t) != 0) return 1;

    printf("Loaded student: %s, %d, GPA %.2f\n", t.name, t.age, t.gpa);

    long sz = file_size(binname);
    if (sz >= 0) {
        printf("Binary file size: %ld bytes\n", sz);
    }
    return 0;
}