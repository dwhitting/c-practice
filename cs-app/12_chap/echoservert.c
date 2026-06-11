#include "networks.h"

void *thread(void *vargp);

int main(int argc, char **argv)
{
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);

    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfdp = malloc(sizeof(int)); 
        if (connfdp == NULL) {
            fprintf(stderr, "failed malloc\n");
            exit(1);
        }
        *connfdp = accept(listenfd, (SA *)&clientaddr, &clientlen);
        printf("connected: %d\n", *connfdp);
        pthread_create(&tid, NULL, thread, connfdp);
    }
}

/* thread routine */
void *thread(void *vargp) {
    int connfd = *((int *) vargp);
    pthread_detach(pthread_self());
    free(vargp);
    echo(connfd);
    printf("closing %d\n", connfd);
    close(connfd);
    return NULL;
}