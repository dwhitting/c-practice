#include "wrap_prox.h"

int main(int argc, char **argv) 
{
    if (argc != 3) {
        printf("Usage: %s <host> <port>\n", argv[0]);
        exit(1);
    }

    char *host = argv[1];
    char *port = argv[2];
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;          /* IPv4 only */
    hints.ai_socktype = SOCK_STREAM;    /* TCP */

    /* address resolution */
    struct addrinfo *results, *ind_res;
    int connect_res;
    if ((connect_res = getaddrinfo(host, port, &hints, &results)) != 0) {
        fprintf(stderr, "getaddrinfo err, %s\n", gai_strerror(connect_res));
        exit(1);
    }

    /* create connection endpoint */
    int client_fd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    if (client_fd < 0) {
        fprintf(stderr, "socket creation failed, err: %d\n", errno);
        free(results);
        exit(1);
    }

    /* establish TCP handshake */
    int status = connect(client_fd, results->ai_addr, results->ai_addrlen);
    if (status < 0) {
        fprintf(stderr, "connect failed, err: %d\n", errno);
        close(client_fd);
        free(results);
        exit(1);
    }

    /* connection establsihed, free connection linked-list */
    freeaddrinfo(results);

    /* transmit request */
    char request_string[MAXLINE];
    sprintf(request_string, "GET / HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", host);
    net_write_n(client_fd, request_string, strlen(request_string));

    char response_string[MAXLINE];
    int bytes_read = 0;
    rio_t rio;
    rio_readinitb(&rio, client_fd);
    while ((bytes_read = rio_readlineb(&rio, response_string, MAXLINE)) > 0) {
        printf("%s", response_string);
        if (strcmp(response_string, "\r\n") == 0)
            break;
    }
    if (bytes_read < 0) {
        fprintf(stderr, "response read error: %d\n", errno);
        exit(1);
    }

    return 0;
}

