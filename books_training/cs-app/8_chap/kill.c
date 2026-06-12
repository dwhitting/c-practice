#include "../wrappers/wrappers.h"

int main(void)
{
    pid_t pid;

    /* child sleeps until SIGKILL signal is received, then dies */
    if ((pid = Fork()) ==0) {
        printf("child pid: %d\n", getpid());
        printf("child group pid: %d\n", getgid());
        fflush(stdout);
        pause(); /* wait for a signal to arrive */
        printf("control should never reach here,\n");
        exit(0);
    } 

    /* parent sinds a SIGKILL signal to a child */
    sleep(1);
    Kill(pid, SIGKILL);
    printf("parent group id: %d\n", getgid());

    exit(0);
}