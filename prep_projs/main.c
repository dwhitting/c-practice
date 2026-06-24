#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        char *args[] = {"tmux", "split-window", "-h", "top", NULL};
        execvp(args[0], args);

        perror("execvp failed");
        exit(1);
    }

    return 0;
}