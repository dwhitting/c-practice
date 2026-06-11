#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


void *thread(void *vargp);

int main(void)
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread, NULL);
    pthread_join(tid, NULL);
    return 0;
}

void *thread(void *vargp) {         /* thread routine */
    printf("Hello\n");
    return NULL;
} 