# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
TARGET = my_io

# Default rule to build the program
all: $(TARGET)

$(TARGET): main.c IO_code.c
	$(CC) $(CFLAGS) IO_code.c main.c -o $(TARGET)

# Clean rule to remove compiled binaries
clean:
	rm -f $(TARGET)
