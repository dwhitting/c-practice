# Variables
CC = gcc #x86_64-linux-gnu-gcc
CFLAGS = -g -Og

# Target
all: word_freq

# Target prereqs
word_freq: main.c util.c wfreq.c
	$(CC) $(CFLAGS) -o word_freq main.c util.c wfreq.c 

# Clean target to remove generated files
clean:
	rm -f word_freq