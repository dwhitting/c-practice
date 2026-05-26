#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int opening_file(void)
{
    char select = 'd';
    int fd;
    #define MAX_READ 20
    char buffer[MAX_READ + 1];

    switch (select) {
        case 'a':
                // read only
            fd = open("startup", O_RDONLY);
            if (fd == -1) {
                printf("failed to open\n");
                char *s_err = strerror(errno);
                printf("errno %d, error str: %s\n", errno, s_err);
                return -1;
            }
            printf("opened R only, %d\n", fd);
            close(fd);
        break;

        case 'b':
                // read and write, trucate to zero bytes
            fd = open("my_file", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR |
                                S_IWUSR);
            if (fd == -1) {
                printf("failed to open\n");
                return -1;
            }
            printf("opened USR WR, %d\n", fd);
            close(fd);

        break;

        case 'c':
            fd = open("w.log", O_WRONLY | O_CREAT | O_APPEND, 
                        S_IRUSR | S_IWUSR);
            if (fd == -1) {
                printf("failed to open (c)\n");
                return -1;
            }
            printf("opened new or existing write-append, %d\n", fd);
            close(fd);
            
        break;

        case 'd':
            ssize_t bytes_read = read(STDIN_FILENO, buffer, MAX_READ);
            if (bytes_read == -1) {
                printf("error reading, errno: %d\n", errno);
                return -1;
            }
            buffer[bytes_read] = '\0';
            printf("read data: %s\n", buffer);
            for (int i = 0; i < 20; i++)
                printf("%d ", buffer[i]);
            printf("\n");

        break;
    }



    return 0;
}