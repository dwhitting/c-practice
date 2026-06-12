#include <stdio.h>
#include <pthread.h>
#include <string.h>

void *thread(void *vargp);

typedef struct {
    int y;
    char chs[10];
} threadArgs;

int thread_prac(void) {

    threadArgs pass_th;
    pass_th.y = 7;
    strcpy(pass_th.chs, "words");


    pthread_t tid;
    pthread_create(&tid, NULL, thread, &pass_th);
    pthread_join(tid, NULL);

    return 0;
}

void *thread(void *vargp) {

    threadArgs *data = vargp;

    printf("Hello!!\n");
    printf("res y: %d, res: chs: %s\n", data->y, data->chs);
    return NULL;
}