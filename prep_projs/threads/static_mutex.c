#include "wrap.h"

typedef struct {
    int thrd_id;
    int ret_sum;
} thread_data_t;

static volatile int glob = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *threadFunc(void *arg) {    /* loop arg times inc glob */
    int loops = *((int *)arg);
    int loc, j, s;

    for (j = 0; j < loops; j++) {
        s = pthread_mutex_lock(&mtx);
        if (s != 0) {
            printf("mutex lock err\n");
            exit(1);
        }
        loc =  glob;
        loc++;
        glob = loc;
        s = pthread_mutex_unlock(&mtx);
        if (s != 0) {
            printf("mutex unlock err\n");
            exit(1);
        }
    }
    
    return NULL;
}

int main(void) 
{
    pthread_t t1, t2;
    int loops, s;

    loops = 1000000;
    s = Pthread_create(&t1, NULL, threadFunc, &loops);
    s = Pthread_create(&t2, NULL, threadFunc, &loops);

    s = Pthread_join(t1, NULL);
    s = Pthread_join(t2, NULL);

    printf("glob = %d\n", glob);

    return 0;
}

