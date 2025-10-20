# Week 5 – Files & Modular Programming
CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 -O2

# Binaries
BIN1=week5_task1_file_io
BIN2=week5_task2_struct_save_load
BIN3=week5_task3_student_management_system
BIN4=week5_task4_struct_binary_io

all: $(BIN1) $(BIN2) $(BIN3) $(BIN4)

$(BIN1): week5_task1_file_io.c
	$(CC) $(CFLAGS) $< -o $@

$(BIN2): week5_task2_struct_save_load.c
	$(CC) $(CFLAGS) $< -o $@

$(BIN3): week5_task3_student_management_system.c
	$(CC) $(CFLAGS) $< -o $@

$(BIN4): week5_task4_struct_binary_io.c
	$(CC) $(CFLAGS) $< -o $@

run1: $(BIN1)
	./$(BIN1)

run2: $(BIN2)
	./$(BIN2)

run3: $(BIN3)
	./$(BIN3)

run4: $(BIN4)
	./$(BIN4)

lab5: all

clean:
	rm -f $(BIN1) $(BIN2) $(BIN3) $(BIN4) *.o