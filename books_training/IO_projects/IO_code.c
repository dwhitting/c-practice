#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#define MAX_CHAR 255

int copy_file(int argc, char *argv[]) {
    
    if (argc < 4) {
        printf("there needs to be two file names for copy\n");
        return -1;
    }

    int fd_read = open(argv[2], O_RDONLY);
    if (fd_read == -1) {
        printf("errno %d, for file: %s. %s\n", errno, argv[1], strerror(errno));
        return -1;
    }

    int fd_write = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd_write == -1) {
        printf("errno %d, for file: %s. %s\n", errno, argv[1], strerror(errno));
        close(fd_read);
        return -1;       
    }

    char buffer[MAX_CHAR + 1];
    char char_read;
    ssize_t bytes_read, bytes_written;
    ssize_t counter = 0;
    int lines_read = 0;

    while ((bytes_read = read(fd_read, &char_read, 1)) > 0) {
        buffer[counter++] = char_read;

        if (char_read == '\n') {
            buffer[counter] = '\0';
            //printf("line: %s", buffer);
            bytes_written = write(fd_write, buffer, counter);
            if (bytes_written == -1) {
                printf("errno %d, for file: %s. %s\n", errno, argv[1], strerror(errno));
                close(fd_read);
                close(fd_write);
                return -1;
            }
            counter = 0;
            lines_read++;
        } 
        else if (counter == MAX_CHAR) {
            buffer[MAX_CHAR] = '\0';
            printf("line %d buffer maxed out at %d chars\n", lines_read, MAX_CHAR);
            bytes_written = write(fd_write, buffer, counter);
            if (bytes_written == -1) {
                printf("errno %d, for file: %s. %s\n", errno, argv[1], strerror(errno));
                close(fd_read);
                close(fd_write);
                return -1;
            }
            counter = 0;
        }    
    }

    if (counter != 0){
        buffer[counter] = '\0';
        write(fd_write, buffer, counter);
        lines_read++;
    }

    if (bytes_read == -1) {
        printf("errno %d, %s\n", errno, strerror(errno));
        close(fd_read);
        close(fd_write);
        return -1;
    }

    close(fd_read);
    close(fd_write);

    return 0;
}