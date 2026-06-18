# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -pthread -Og
TARGET = main

# Default rule to build the program
all: $(TARGET)

$(TARGET): main.c 
	$(CC) $(CFLAGS) ../my_timer/my_timer.c main.c -o $(TARGET)

# Clean rule to remove compiled binaries
clean:
	rm -f $(TARGET)
