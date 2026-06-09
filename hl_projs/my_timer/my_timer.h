#ifndef MY_TIMER_H
#define MY_TIMER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define mins(x) (x / 60)
#define milli(x) (x / 1000000.0)

typedef struct __my_timer_t {
    struct timespec t_spec_begin, t_spec_end;
} my_timer_t; 

void begin_timer(my_timer_t *in_timer);
void end_timer(my_timer_t *in_timer) ;

#endif