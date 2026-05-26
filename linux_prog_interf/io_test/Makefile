# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
TARGET = main

# Default rule to build the program
all: $(TARGET)

$(TARGET): main.c opening_file.c
	$(CC) $(CFLAGS) opening_file.c main.c -o $(TARGET)

# Clean rule to remove compiled binaries
clean:
	rm -f $(TARGET)
