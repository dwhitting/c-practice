#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
    // for high precision
    struct timespec ts_1, ts_2;
    timespec_get(&ts_1, TIME_UTC);

    struct tm *t = localtime(&ts_1.tv_sec);
    long millisec = ts_1.tv_nsec / 1000000;

    printf("output, min: %d, sec: %d\n",t->tm_min, t->tm_sec);
    printf("millisec 1: %ld\n", millisec);

    struct timespec delay = { .tv_sec = 0, .tv_nsec = 250000000}; //250 millisec

    nanosleep(&delay, NULL);

    timespec_get(&ts_2, TIME_UTC);
    long second_millisec = ts_2.tv_nsec / 1000000;
    printf("millisec 2: %ld\n", second_millisec);

    return 0;
}