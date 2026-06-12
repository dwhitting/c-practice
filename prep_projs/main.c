#include "wrap.h"

void *thread(void *vargp) {
    printf("Hi\n");
    return NULL;
}

int main(void) 
{
    pthread_t tid;

    Pthread_create(&tid, NULL, thread, NULL);
    pthread_join(tid, NULL);

    return 0;
}

