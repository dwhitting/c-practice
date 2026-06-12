//#include "csapp.h"
#include <unistd.h>
#include <stdio.h>



int main (void)
{
    char c;
    int chars;

    while(read(STDIN_FILENO, &c, 1) != 0)
        write(STDOUT_FILENO, &c, 1);

    return 0;
}