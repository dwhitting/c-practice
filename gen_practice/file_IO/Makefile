# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
TARGET = main

# Default rule to build the program
all: $(TARGET)

$(TARGET): struct_file.c read_from_file.c write_to_file.c main.c
	$(CC) $(CFLAGS) struct_file.c read_from_file.c write_to_file.c main.c -o $(TARGET)

# Clean rule to remove compiled binaries
clean:
	rm -f $(TARGET)
