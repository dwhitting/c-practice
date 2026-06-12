#include "networks.h"
#include "sbuf.h"

/* create an empty, bounded, shared FIFO buffer with n slots */
void sbuf_init(sbuf_t *sp, int n) {
    sp->buf = calloc(n, sizeof(int));
    sp->n = n;                  /* buffer holds max of n itmes */
    sp->front = sp->rear;       /* empty buffer iff front == rear */
    sem_init(&sp->mutex, 0, 1); /* binary semaphor for locking */
    sem_init(&sp->slots, 0, n); /* initially, buf has n empty slots */
    sem_init(&sp->items, 0, 0); /* initially, buf has zero data items */
}

/* clean up buffer sp */
void sbuf_deinit(sbuf_t *sp) {
    free(sp->buf);
}

/* insert items onto the rear of shared buffer sp*/
void sbuf_insert(sbuf_t *sp, int item) {
    sem_wait(&sp->slots);                   /* wait for available slot */
    sem_wait(&sp->mutex);                   /* lock the buffer */
    sp->buf[(++sp->rear)%(sp->n)] = item;   /* insert item */
    sem_post(&sp->mutex);                   /* unlock buffer */
    sem_post(&sp->items);                   /* announce avail item */
}

/* remove and retunr first item from buffer sp */
int sbuf_remove(sbuf_t *sp) {
    int item;                               
    sem_wait(&sp->items);                   /* wait for avail item */
    sem_wait(&sp->mutex);                   /* lock the buffer */
    item = sp->buf[(++sp->front)%(sp->n)];  /* remove the item */
    sem_post(&sp->mutex);                   /* unlock the buffer */
    sem_post(&sp->slots);                   /* announce avail slot */
    return item;            
}