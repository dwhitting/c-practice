#include "networks.h"

/* this includes buggy code */

void *thread(void *vargp); 

/* global shared var */
volatile long cnt =0;       /* counter */

int main(int argc, char **argv)
{
    long niters;
    pthread_t tid1, tid2;

    /* check input argument */
    if (argc != 2) {
        printf("usage: %s <niters>\n", argv[0]);
        exit(0);
    }
    niters = atoi(argv[1]);

    /* create threads and wait for them to finish */
    pthread_create(&tid1, NULL, thread, &niters);
    pthread_create(&tid2, NULL, thread, &niters);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    /* check result */
    if (cnt != (2 * niters))
        printf("Boom! cnt=%ld\n", cnt);
    else
        printf("Ok cnt=%ld\n", cnt);
    exit(0);
}

/* thread routine */
void  *thread(void *vargp) {
    long i, niters = *((long *)vargp);

    for (i = 0; i < niters; i++)
        cnt++;

    return NULL;
}