#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/unistd.h>

int main(int argc, char *argv[])
{
    long long the_arr[atoi(argv[1])];
    int time_sec = atoi(argv[2]);
    pid_t proc = getpid();

    printf("Doing %d int(s) for %d seconds\n", atoi(argv[1]), time_sec);
    printf("pid: %d\n", proc);

    long long dum_var;
    int inc = 0;
    time_t start_time = time(NULL);
    while (difftime(time(NULL), start_time) < time_sec) {
        dum_var = the_arr[inc++];
        if (inc == atoi(argv[1])) inc = 0;
    }

    printf("mem user is done.\n");

    return 0;
}