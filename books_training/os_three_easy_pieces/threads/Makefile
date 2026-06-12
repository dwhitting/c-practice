# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
TARGET = main

# Default rule to build the program
all: $(TARGET)

$(TARGET): main.c thread_0.c thread_1.c thread_prac.c thread_creation.c
	$(CC) $(CFLAGS) thread_creation.c thread_0.c thread_1.c thread_prac.c main.c -o $(TARGET)

# Clean rule to remove compiled binaries
clean:
	rm -f $(TARGET)
