# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
TARGET = main

# Default rule to build the program
all: $(TARGET)

$(TARGET): my_get_str.c my_len.c my_get_char.c main.c 
	$(CC) $(CFLAGS) my_get_str.c my_len.c my_get_char.c main.c -o $(TARGET)

# Clean rule to remove compiled binaries
clean:
	rm -f $(TARGET)
