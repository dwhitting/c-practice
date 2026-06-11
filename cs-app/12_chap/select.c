#include "networks.h"

void command(void);

int main (int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    fd_set read_set, ready_set;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = open_listenfd(argv[1]);

    FD_ZERO(&read_set);                 /* clear read set */
    FD_SET(STDIN_FILENO, &read_set);    /* add stdin to read set */
    FD_SET(listenfd, &read_set);        /* add listenfd to read set */

    while (1) {
        ready_set = read_set;
        select(listenfd + 1, &ready_set, NULL, NULL, NULL);
        if (FD_ISSET(STDIN_FILENO, &ready_set))
            command();  /* read command line from stdin */
        if (FD_ISSET(listenfd, &ready_set)) {
            clientlen = sizeof(struct sockaddr_storage);
            connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
            printf("Connection made, fd: %d\n", connfd);
            echo(connfd);       /* echo cleint input until EOF */
            printf("Closing fd: %d\n", connfd);
            close(connfd);
        }
    }
}

void command(void) {
    char buf[MAXLINE];
    if (!fgets(buf, MAXLINE, stdin))
        exit(0); /* EOF */
    printf("%s", buf);      /* process the input command */
}