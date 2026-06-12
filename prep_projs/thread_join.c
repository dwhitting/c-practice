#include "wrap.h"

void *thread(void *vargp) {
    int i = *(int *)vargp;
    printf("Hi from thread %d, tid %ld\n", i, pthread_self());
    return NULL;
}

int main(void) 
{
    pthread_t tid_holder[10];
    int arg_holder[10];

    for (int i = 0; i < 10; i++) {
        arg_holder[i] = i;
        Pthread_create(&tid_holder[i], NULL, thread, &arg_holder[i]);
    }

    for (int i = 0; i < 10; i++)
        pthread_join(tid_holder[i], NULL);

    return 0;
}

