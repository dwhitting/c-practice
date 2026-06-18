#include "my_timer.h"

/* This is the timer I made to measure processing time.
 * See my_timer.h for a little more detail */

/* I call this function to begin the timer */
void begin_timer(my_timer_t *in_timer) {
    /* get current time and place it in my_timer_t stuct */
    timespec_get(&(in_timer->t_spec_begin), TIME_UTC);
    printf("timer started\n");

}

/* This function ends the timer and prints the elaplsed time in minuts, 
 * seconds, and milliseconds.  I could adjust what nsec_diff is divided by
 * to go as far down as nanoseconds, if needed */
void end_timer(my_timer_t *in_timer) {

    /* get current timer and place it int the end time part fo my_timer_t struct */
    timespec_get(&(in_timer->t_spec_end), TIME_UTC);
    printf("End timer\n");

    /* calc time diffs */
    long secs_diff = in_timer->t_spec_end.tv_sec - in_timer->t_spec_begin.tv_sec;
    long nsec_diff = in_timer->t_spec_end.tv_nsec - in_timer->t_spec_begin.tv_nsec;

    /* If begin nanosec is larger than end nanosec, result is a negative nsec_diff. 
     * This calc 'barrows' one second from secs_diff and adds it to nsec_diff, simliar to 
     * borrowing in long division */
    if (nsec_diff < 0) {
        secs_diff -= 1;
        nsec_diff += 1000000000L;
    }

    long mins = secs_diff / 60;
    long secs = secs_diff % 60;
    float millis = (float)nsec_diff / 1000000.0f;

    in_timer->min_data = mins;
    in_timer->sec_data = secs;
    in_timer->milli_data = millis;

    printf("Mins: %ld, secs: %ld, millis: %.2f\n", mins, secs, millis);
}