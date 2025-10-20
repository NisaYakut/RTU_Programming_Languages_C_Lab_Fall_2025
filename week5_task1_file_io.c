// Week 5 – Task 1: File I/O basics
// Author: Nisa Yakut (replace with your Student ID if needed)
// Build: gcc -Wall -Wextra -Werror -std=c11 -O2 week5_task1_file_io.c -o week5_task1_file_io
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    const char *fname = "data.txt";
    FILE *fp = fopen(fname, "w");
    if (fp == NULL) {
        perror("fopen for write");
        return 1;
    }

    printf("Writing lines to %s...\n", fname);
    // You may also read from user and write; here we write a couple of lines per example.
    fprintf(fp, "Hello, file I/O in C!\n");
    fprintf(fp, "This is another line.\n");
    fclose(fp);

    fp = fopen(fname, "r");
    if (fp == NULL) {
        perror("fopen for read");
        return 1;
    }

    printf("Reading contents:\n");
    char buf[256];
    while (fgets(buf, (int)sizeof buf, fp) != NULL) {
        fputs(buf, stdout);
    }

    if (ferror(fp)) {
        perror("read error");
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}