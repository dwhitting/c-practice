#include "my_timer.h"

void begin_timer(my_timer_t *in_timer) {
    timespec_get(&(in_timer->t_spec_begin), TIME_UTC);
    printf("timer started\n");

}

void end_timer(my_timer_t *in_timer) {
    timespec_get(&(in_timer->t_spec_end), TIME_UTC);
    printf("End timer\n");
    long mins = mins(in_timer->t_spec_end.tv_sec) - mins(in_timer->t_spec_begin.tv_sec);
    long secs = in_timer->t_spec_end.tv_sec - in_timer->t_spec_begin.tv_sec;
    float millis = milli(in_timer->t_spec_end.tv_nsec) - milli(in_timer->t_spec_begin.tv_nsec);

    printf("Mins: %ld, secs: %ld, millis: %.2f\n", mins, secs, millis);

    // printf("Min: %ld, sec: %ld, milli: %f\n", mins(in_timer->t_spec_begin.tv_sec),
    //         in_timer->t_spec_begin.tv_sec, milli(in_timer->t_spec_begin.tv_nsec));

}