#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#define N 10



int main(void)
{
    pid_t pid;
    int status;

    close(STDOUT_FILENO);
    open("fork_to_file.output", O_CREAT | O_WRONLY | \
        O_TRUNC | O_APPEND, S_IRWXU);

    for (int i = 0; i < N; i++) {
        pid = fork();
            if (pid == 0)
                return 100 + i;
    }

    int parent_pid;
    if (pid > 0) {
        //printf("parent (pid:%d) exit\n", (int) getpid());
        parent_pid = (int) getpid();
        //return 0;
    }
        

    while ((pid = waitpid(-1, &status, 0)) > 0 ) {
        if (WIFEXITED(status))
            printf("child %d terminated normally " \
                "with exit status = %d\n", pid, WEXITSTATUS(status));
        else    
            printf("child %d terminated abnormally\n", pid);
    }


    return 0;
}