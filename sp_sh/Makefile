# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -pthread 
DEPS = stan_hdr.h 
OBJS = main.o stan_hdr.o bnk_accts.o
TARGET = main

# Default rule to build the program
all: $(TARGET)

# 1. The Target depends on all the object (.o) files, not just main.c
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# 2. Rule to compile each .c file into a .o file
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove compiled binaries AND object files
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean