# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
TARGET = main

# Default rule to build the program
all: $(TARGET)

$(TARGET): bin_experiments.c binbin.c main.c
	$(CC) $(CFLAGS) bin_experiments.c binbin.c main.c -o $(TARGET)

# Clean rule to remove compiled binaries
clean:
	rm -f $(TARGET)
