#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#define CHARNUM 255


int main(void)
{
    char buff[CHARNUM];
    char *input;

    input = fgets(buff, CHARNUM, stdin);

    int fd;
    fd = open("some_file.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR |
                                S_IWUSR);
    input[strlen(input) - 1] = '\0';
    write(fd, buff, strlen(input));

    lseek(fd, 0, SEEK_SET);
    
    char buff_2[CHARNUM];
    int bytes_read;
    bytes_read = read(fd, buff_2, CHARNUM -1);
    buff_2[bytes_read] = '\0';
    printf("output: %s\n", buff_2);

    close(fd);

    return 0;
}