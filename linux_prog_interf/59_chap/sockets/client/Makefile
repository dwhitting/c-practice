# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g  -I../headers -Wno-format-truncation
TARGET = 16d_ucase_cl

# Default rule to build the program
all: $(TARGET)

$(TARGET): 16d_ucase_cl.c ../../../headers/error_functions.c
	$(CC) $(CFLAGS) ../../../headers/error_functions.c 16d_ucase_cl.c -o $(TARGET) 

# Clean rule to remove compiled binaries
clean:
	rm -f $(TARGET)
