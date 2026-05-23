#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int fd;
    fd = open("startup", O_RDONLY);
    if (fd == -1) {
        printf("failed to open\n");
        return -1;
    }
    
    printf("opened, %d\n", fd);
    close(fd);

    return 0;
}