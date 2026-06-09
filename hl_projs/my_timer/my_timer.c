#include "my_timer.h"

void begin_timer(my_timer_t *in_timer) {
    timespec_get(&(in_timer->t_spec_begin), TIME_UTC);
    printf("timer started\n");

}

void end_timer(my_timer_t *in_timer) {
    timespec_get(&(in_timer->t_spec_end), TIME_UTC);
    printf("End timer\n");

    long secs_diff = in_timer->t_spec_end.tv_sec - in_timer->t_spec_begin.tv_sec;
    long nsec_diff = in_timer->t_spec_end.tv_nsec - in_timer->t_spec_begin.tv_nsec;

    if (nsec_diff < 0) {
        secs_diff -= 1;
        nsec_diff += 1000000000L;
    }

    long mins = secs_diff / 60;
    long secs = secs_diff % 60;
    float millis = (float)nsec_diff / 1000000.0f;

    printf("Mins: %ld, secs: %ld, millis: %.2f\n", mins, secs, millis);
}