# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g  -I../headers -Wno-format-truncation
TARGET = main

# Default rule to build the program
all: $(TARGET)

$(TARGET): 16d_ucase_sv.c ../../headers/error_functions.c
	$(CC) $(CFLAGS) ../../headers/error_functions.c 16d_ucase_sv.c -o $(TARGET) 

# Clean rule to remove compiled binaries
clean:
	rm -f $(TARGET)
