# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -I../headers -Wno-format-truncation
TARGET = copy

# Default rule to build the program
all: $(TARGET)

$(TARGET): copy.c ../headers/error_functions.c
	$(CC) $(CFLAGS) ../headers/error_functions.c copy.c -o $(TARGET) 

# Clean rule to remove compiled binaries
clean:
	rm -f $(TARGET)
# to clean files: rm -f copy *.o a.out
