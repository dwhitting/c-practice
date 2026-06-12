# Variables
CC = gcc #x86_64-linux-gnu-gcc
CFLAGS = -g -Og #-static

# Target
all: wfreq

# Target prereqs
wfreq: main.c util.c wfreq.c
	$(CC) $(CFLAGS) -o wfreq main.c util.c wfreq.c 

# Clean target to remove generated files
clean:
	rm -f wfreq