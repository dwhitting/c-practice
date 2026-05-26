# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -I../headers -Wno-format-truncation
TARGET = read_line

# Default rule to build the program
all: $(TARGET)

$(TARGET): read_line.c main.c
	$(CC) $(CFLAGS) read_line.c main.c -o $(TARGET) 

# Clean rule to remove compiled binaries
clean:
	rm -f $(TARGET)
