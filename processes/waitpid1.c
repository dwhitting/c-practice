#include <sys/types.h>
#include <sys/wait.h>
#include "forks.c"

#define N 5

int main(void)
{
    int status, i;
    pid_t pid;
    
     //parent creates N children
    for (i = 0; i < N; i++)
        if ((pid = Fork()) == 0) //Child
            return 100 + i;

    //Parent reaps N children in no partiuclar order
    while ((pid = waitpid(-1, &status, 0)) > 0) {
        if (WIFEXITED(status))
            printf("child %d terminated normally " \
                "with exit status = %d\n", pid, WEXITSTATUS(status));
        else    
            printf("child %d terminated abnormally\n", pid);
    }

    // The only normal terinatin is if there are no more children
    if (errno != ECHILD)
        unix_error("waitpid error");

    return 0;
}