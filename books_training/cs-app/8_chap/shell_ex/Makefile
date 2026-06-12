# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -pthread -Og
TARGET = shellex

# Default rule to build the program
all: $(TARGET)

$(TARGET): shellex.c   
	$(CC) $(CFLAGS) ../../wrappers/wrappers.c shellex.c -o $(TARGET)

# Clean rule to remove compiled binaries
clean:
	rm -f $(TARGET)
