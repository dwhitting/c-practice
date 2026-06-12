#include "../wrappers/wrappers.h"

void sigint_handler(int sig) {   /* SIGINT handler */
    printf("caugt SIGINT: %d\n", sig);
    return;
}

int main(void) 
{
    /* install SIGINT handler */
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
        unix_error("signal error");

    pause();  /* wait for the receipt of the signal */

    printf("returned from SIGINT\n");

    char intake[MAXBUF];
    char *fgets_in;
    while ((fgets_in = Fgets(intake, MAXBUF, stdin)) != NULL) {

        if (strcmp(intake, "quit\n") == 0)
            break;

        /* install SIGINT handler */
        if (signal(SIGINT, sigint_handler) == SIG_ERR)
            unix_error("signal error");

        pause();  /* wait for the receipt of the signal */

        printf("returned from SIGINT\n");
    }
        

    return 0;
}