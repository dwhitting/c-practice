#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0 ) {
        fprintf(stderr, "fork failed\n");
        return 1;
    } else if (rc == 0) { // Child (new process)
        close(STDOUT_FILENO);
        open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        // now exec "wc" ...
        char *myargs[3];
        myargs[0] = strdup("wc");                // program "wc" (word count)
        myargs[1] = strdup("calling_fork_p4.c"); // argument: file to count
        myargs[2] = NULL;                        // marks end of array
        execvp(myargs[0], myargs);               // runs word count
        printf("this shouldn't pring out");
    } else { // Parent goes down this path (main)
        int rc_wait = wait(NULL);
    }

    return 0;
}