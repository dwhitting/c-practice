#include "my_timer.h"


int main(void)
{
    my_timer_t timer;

    begin_timer(&timer);

    sleep(2);

    end_timer(&timer);
    

    return 0;
}