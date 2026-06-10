#include "my_timer.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    char template[] = "/tmp/tempFileXXXXXX";
    char test[50] = {0};
    char read_string[100] = {0};

    setbuf(stdout, NULL);
    
    int fd = mkstemp(template);
    printf("file name: %s\n", template);

    pid_t child_pid, parent_pid;

    switch (child_pid = fork()) {
        
        case 0:
            strcpy(test, "child first\n");
            write(fd, test, strlen(test));
            exit(0);
        default:  /* parent */
            parent_pid = getpid();
            strcpy(test, "parent second\n");
            write(fd, test, strlen(test));
            wait(NULL);
            
    }
    printf("child pid: %d\n", child_pid);
    printf("parent pid: %d\n", parent_pid);
    lseek(fd, 0, SEEK_SET);
  
    ssize_t bytes_read;
    char *read_ptr = read_string;
    
    while ((bytes_read = read(fd, test, sizeof(test) - 1)) != 0) {
        test[bytes_read] = '\0';
        memcpy(read_ptr, test, bytes_read);
        read_ptr += bytes_read;
    }
    
    *read_ptr = '\0';
    printf("bytes: %ld\n", strlen(read_string));
    printf("string: %s\n", read_string);
    return 0;    
}