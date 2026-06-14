#include "wrap_prox.h"

int main(int argc, char **argv) 
{
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    char *port  = argv[1];
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;          /* IPv4 */
    hints.ai_socktype = SOCK_STREAM;    /* TCP */
    hints.ai_flags = AI_PASSIVE;        /* tell addrinfo to bind to any interface (0.0.0.0) */

    /* address resultion */
    struct addrinfo *res;

    /* pass NULL for host because AI_PASSIVE binds locally to the machine */
    int status = getaddrinfo(NULL, port, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }
    

    return 0;
}

