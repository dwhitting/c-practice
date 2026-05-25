#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define MAX_CHAR 255

int copy_file(int argc, char *argv[]) {
    
    if (argc < 4) {
        printf("there needs to be two file names for copy\n");
        return -1;
    }

    int fd_open = open(argv[2], O_RDONLY);
    if (fd_open == -1) {
        printf("errno %d, for file: %s. %s\n", errno, argv[1], strerror(errno));
        return -1;
    }

    char buffer[MAX_CHAR + 1];
    char char_read;
    ssize_t bytes_read;
    int counter = 0;

    while ((bytes_read = read(fd_open, &char_read, 1)) > 0) {
        buffer[counter++] = char_read;

        //  ************ if (counter == MAX_CHAR(?).......)

        if (char_read == '\n') {
            buffer[counter] = '\0';
            printf("line: %s", buffer);
            counter = 0;
        }        
    }

    if (bytes_read == -1) {
        printf("errno %d, %s\n", errno, strerror(errno));
        return -1;
    }

    

    return 0;
}