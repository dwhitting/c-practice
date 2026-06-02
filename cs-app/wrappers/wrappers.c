#include "wrappers.h"

int unix_error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    return 0;
}

int app_error(char *msg) { /* application error */
    fprintf(stderr, "%s\n", msg);
    return 0;
}

pid_t Fork(void){
    pid_t pid;
    if ((pid = fork()) < 0)
        unix_error("Fork error");
    
    return pid;
}

char *Fgets(char *ptr, int n, FILE *stream) {
    char *rptr;
    if ((rptr = fgets(ptr, n, stream)) == NULL)
        app_error("Fgets error");

    return rptr;
}

void Kill(pid_t pid, int signum) {
    int rc;
    if ((rc = kill(pid, signum)) < 0)
        unix_error("Kill error");
}
