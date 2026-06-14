#include "wrap_prox.h"

int main(int argc, char **argv) 
{
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    char *port  = argv[1];
    int status;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;          /* IPv4 */
    hints.ai_socktype = SOCK_STREAM;    /* TCP */
    hints.ai_flags = AI_PASSIVE;        /* tell addrinfo to bind to any interface (0.0.0.0) */

    /* address resultion */
    struct addrinfo *res;

    /* pass NULL for host because AI_PASSIVE binds locally to the machine */
    status = getaddrinfo(NULL, port, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }

    //check_serv_connection(res);   /* if want to check host and port */

    /* create listening endpoint */
    int listen_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listen_fd < 0) {
        fprintf(stderr, "listening socket creation failed: %d\n", errno);
        free(res);
        exit(1);
    }

    /* binding  the port */
    status = bind(listen_fd, res->ai_addr, res->ai_addrlen);
    if (status < 0) {
        fprintf(stderr, "bind failed, port may be in use: %d\n", errno);
        close(listen_fd);
        free(res);
        exit(1);
    }   
    free(res);

    /* enable incoming queue, converst passive socket to active listeniing */
    status = listen(listen_fd, LISTENQ);
    if (status < 0) {
        fprintf(stderr, "listen failed: %d\n", errno);
        close(listen_fd);
        exit(1);
    }  
    printf("Server is listening on port: %s\n", port);

    /* iterative server loop */
    struct sockaddr_storage client_addr;
    int conn_fd;
    while (1) {
        socklen_t client_len = sizeof(client_addr);
        conn_fd = accept(listen_fd,(struct sockaddr *)&client_addr, &client_len);
        if (conn_fd < 0) {
            printf("accept failed, attempting next connection\n");
            continue;
        }
        printf("Connected to client\n");
        if (client_addr.ss_family == AF_INET) {
            struct sockaddr_in *t_addr =  (struct sockaddr_in  *)&client_addr;
            check_client_connection(t_addr, client_len);
        }
        close(conn_fd);
    }
        
    close(listen_fd);
    
    return 0;
}

void check_client_connection(struct sockaddr_in *s_addr, int s_len) {
    char client_IP[MAXLINE];
    char client_port[MAXLINE];
    int name_stat = getnameinfo((struct sockaddr *) s_addr, s_len, 
            client_IP, sizeof(client_IP),
            client_port, sizeof(client_port),
            NI_NUMERICHOST | NI_NUMERICSERV);
    if (name_stat == 0)
        printf("Client IP %s, Port: %s\n", client_IP, client_port);
}

void check_serv_connection(struct addrinfo *res) {
    char host_buf[MAXLINE];
    char serv_buf[MAXLINE];
    int name_stat;
    for (struct addrinfo *sngl = res; sngl != NULL; sngl = sngl->ai_next){
        name_stat = getnameinfo(sngl->ai_addr, sngl->ai_addrlen,
                host_buf, sizeof(host_buf), serv_buf, sizeof(serv_buf),
                NI_NUMERICHOST | NI_NUMERICSERV);
        if (name_stat == 0)
            printf("Resolved host: %s, port: %s\n", host_buf, serv_buf);
    }
}

