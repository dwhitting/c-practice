#include <stdio.h>
#include "../wrappers/wrappers.h"

int main(void)
{

    pid_t pid;
    int x = 1;

    pid = Fork();
    if (pid == 0) {     /* Child procces */
        printf("child: x=%d\n", ++x);
        return 0;
    }
   
    /* Parent */
    printf("parent: x=%d\n", --pid);

    return 0;
}