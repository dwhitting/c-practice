#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int unix_error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    return 0;
}

pid_t Fork(void) 
{
    pid_t pid;

    if ((pid = fork()) < 0)
	unix_error("Fork error");
    return pid;
}

int main(void)
{
    pid_t pid;
    int x = 1;
    
    pid = Fork();
    if (pid == 0) { //child
        printf("child: x=%d\n", ++x);
        return 0;
    }

    //parent
    printf("parent: x=%d\n", --x);

    return 0;
}