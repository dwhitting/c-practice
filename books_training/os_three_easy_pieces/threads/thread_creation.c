#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct __myarg_t {
    int a;
    int b;
} myarg_t;

typedef struct __myret_t {
    int x;
    int y;
} myret_t;


void *mythread(void *arg) {
    myarg_t *m = (myarg_t *) arg;
    printf("%d %d\n", m->a, m->b);
    myret_t *r = malloc(sizeof(myret_t));
    if (r == NULL) return NULL;
    r -> x = 1;
    r -> y = 2 ;
    return (void *) r;
}

int thread_creation(void) {
    pthread_t p;
    myret_t *m;

    myarg_t args = {10, 20};
    pthread_create(&p, NULL, mythread, &args);
    pthread_join(p, (void **) &m);
    printf("returned %d %d\n", m -> x, m -> y);
    free(m);

    return 0;
}


/*  Keeps causing error.  I think because of 64-bit and thinking this must
    have been make for 32-bit.  Seems like too much work to fix at this point.
    no vaule for the buck */

// void *mythread_simple(void *arg) {
//     int m = *((int *) arg);
//     printf("%d\n", m);
//     return (void *) (int *) arg + 1;
// }

// int simple_arg_pass(void) {
//     pthread_t p;
//     void *m;
//     int rc = 100;
//     pthread_create(&p, NULL, mythread_simple, (void *) &rc);
//     pthread_join(p, (void **) &m);
//     printf("result: %d\n", m);

//     return 0;
// }