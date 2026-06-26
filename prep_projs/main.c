#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

static void sigHandler(int sig) {
    printf("Signal hit\n");
}

int main() {
    int j;

    if (signal(SIGINT, sigHandler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    for (j = 0; ; j++) {
        printf("%d\n", j);
        sleep(3);
    }
    

    return 0;
}