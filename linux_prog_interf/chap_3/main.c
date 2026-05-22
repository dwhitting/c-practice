#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(void)
{
    FILE *fd;
    fd = fopen("some file", "r"); 
    if (fd == NULL){
        printf("errno %d\n", errno);
        perror("open");
        char *str_e = strerror(errno);
        printf("\n%s\n", str_e);

        return 1;
    } 
        

    fclose(fd);

    return 0;
}