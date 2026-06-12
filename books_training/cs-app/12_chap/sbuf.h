#include <semaphore.h>

typedef struct {
    int *buf;       /* buffer array */
    int n;          /* max num of slots */
    int front;      /* buf[(front+1)%n] is first item */
    int rear;       /* buf[rear%n] is last item */
    sem_t mutex;    /* protects access to buf */
    sem_t slots;    /* counts avail slots */
    sem_t items;    /* counts avail items */
} sbuf_t;