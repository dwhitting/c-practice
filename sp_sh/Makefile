# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -pthread 
TARGET = main

# Default rule to build the program
all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) bnk_accts.c stan_hdr.c main.c -o $(TARGET)

# Clean rule to remove compiled binaries
clean:
	rm -f $(TARGET)
