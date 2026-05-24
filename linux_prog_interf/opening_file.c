#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int opening_file(void)
{
    char select = 'c';
    int fd;

    switch (select) {
        case 'a':
                // read only
            fd = open("startup", O_RDONLY);
            if (fd == -1) {
                printf("failed to open\n");
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
    }



    return 0;
}