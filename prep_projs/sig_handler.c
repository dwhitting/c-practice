#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

static void sigHandler(int sig) {
    static int count = 0;

    /* Unsafe. Uses non-async sig functions */
    if (sig == SIGINT) {
        count++;
        printf("Caught SIGINT (%d)\n", count);
        return;     /* resumes at point of interupt */
    }

    /* must be SIGQUIT if gets here */
    printf("Caught SIGQUIT\n");
    exit(EXIT_SUCCESS);
}

int main() {

    if (signal(SIGINT, sigHandler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    if (signal(SIGQUIT, sigHandler) == SIG_ERR) {
        perror("signal");
    }

    for (;;)        
        pause();    /* blocks until signal is caught */
    
    return 0;
}