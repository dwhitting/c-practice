#include "my_timer.h"
#include <sys/types.h>
#include <string.h>

int main(void)
{
    char template[] = "/tmp/testXXXXXX";
    char test[20];

    setbuf(stdout, NULL);
    
    int fd = mkstemp(template);
    printf("file name: %s\n", template);

    pid_t proc_pid;

    switch (proc_pid = fork()) {
        
        case 0:
            strcpy(test, "child first\n");
            write(fd, test, 20);
            exit(0);
        default:  /* parent */
            strcpy(test, "parent second\n");
            write(fd, test, 20);
    }
    return 0;    
}