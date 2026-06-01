#include <stdio.h>
#include "../wrappers/wrappers.h"
#include <sys/wait.h>
#define N 10

int main(void)
{

    int status, i;
    pid_t pid[N], retpid;

    /* parent creates N children */
    for (i = 0; i < N; i++)
        if ((pid[i] = Fork()) == 0) { /* child */
    
            return (100 + i);
        }
    
    /* parent reaps N children in no particular order */
    i = 0;
    while ((retpid = waitpid(pid[9 - i++], &status, 0)) > 0) {
        if (WIFEXITED(status))
            printf("child %d terminated normally with exit status %d\n", 
                    retpid, WEXITSTATUS(status));
        else   
            printf("child %d terminated abnormally\n", retpid);
    }

    /* the only normal termination is if there are no more children */
    if (errno != ECHILD)
        unix_error("waitpid error");

    return 0;
}