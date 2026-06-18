#include "wrap_prox.h"

/* Multi-thread Proxy Server
 * I plan to have this as the final stage for this iteration
 * of proxy server projects.  It now spins up thread for 
 * each client connection 
 */

 /* struct that will be malloc'd so each thread has it's own data.
  * most significantly, this ensures each one has a unique client
  * file descripter so the thread to step on each other's descripters 
  */
typedef struct {
    int client_fd;
    char port[100];
    int thread_idx;
} prox_thread_data_t;

void *proxy_thread(void *vargp);

int main(int argc, char **argv) 
{
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    /* Using this as an index to track individual threads */
    int thread_track = 0;

    /* open port to listen for clients */
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

    check_serv_connection(res);   /* if want to check host and port */

    /* create listening endpoint */
    int listen_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listen_fd < 0) {
        fprintf(stderr, "listening socket creation failed: %d\n", errno);
        free(res);
        exit(1);
    }

    /* binding the port */
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
    int client_fd;
    while (1) {
        socklen_t client_len = sizeof(client_addr);
        client_fd = accept(listen_fd,(struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            printf("accept failed, attempting next connection\n");
            continue;
        }
        printf("Connected to client\n");
        if (client_addr.ss_family == AF_INET) {
            struct sockaddr_in *t_addr =  (struct sockaddr_in  *)&client_addr;
            check_client_connection(t_addr, client_len);
        }

        /* Here is where each thread is created and passed to the
         * function: proxy_thread 
         */
        thread_track++;
        pthread_t tid;
        prox_thread_data_t *next_thread = malloc(sizeof(prox_thread_data_t));
        memset(next_thread, 0, sizeof(prox_thread_data_t));
        next_thread->thread_idx = thread_track;
        next_thread->client_fd = client_fd;
        strcpy(next_thread->port, port);
        pthread_create(&tid, NULL, proxy_thread, next_thread);
        printf("\nCreated thread: %d\n", thread_track);
        pthread_detach(tid);
    

        
    }
        
    /* clean up listening port */
    close(listen_fd);
    printf("Port closed\n");
    
    return 0;
}

/* Worker function for each thread 
 * Each return in this function kills a thread since they are detached.
 * I have a printf to show which one is being killed to track that 
 */
void *proxy_thread(void *vargp) {

    /* de-ref vargp to usable data */
    prox_thread_data_t data_in = *(prox_thread_data_t *)vargp;

    /* freeing malloc'd struct so it doesn't leak, since copied to this stack */
    free(vargp);
    int client_fd = data_in.client_fd;
    char port[100];
    strcpy(port, data_in.port);

    rio_t rio_client;
    rio_readinitb(&rio_client, client_fd);
    char net_read[MAXLINE];
    int chars_read;
    int status;

    /* http request pieces */
    char method[MAXLINE];
    char URL[MAXLINE];
    char version[MAXLINE];

    /* outbount proxy pieces */
    char hostname[MAXLINE];
    char target_port[MAXLINE];
    char path_outbound[MAXLINE];
    
    chars_read = rio_readlineb(&rio_client, net_read, sizeof(net_read));
    if (chars_read > 0) {
        printf("Request Line: %s", net_read);
        sscanf(net_read, "%s %s %s", method, URL, version);
        printf("Method: %s, URL: %s, Version: %s", method, URL, version);

        /* safely guard to process only standard GET requests */
        if (strcmp(method, "GET") != 0) {
            printf("Detected non-GET method (%s). Dropping connection\n", method);
            close(client_fd);
            printf("Killing thread: %d\n", data_in.thread_idx);
            return NULL;
        }

        if ((strcmp(URL, "/") == 0)) {
            strcpy(hostname, "127.0.0.1");  /* localhost */
            strcpy(target_port, port);
        } else {
            parse_url(URL, hostname, target_port, path_outbound);
        }

        while (1) {
            chars_read = rio_readlineb(&rio_client, net_read, sizeof(net_read));
            if (chars_read > 0) {
                if (strcmp(net_read, "\r\n") == 0 || strcmp(net_read, "\n") == 0) {
                    printf("Header block drained.\n");
                    break;
                }
            } else {
                break;      /* trap EOF or error inside drain loop */
            }
        }  
        
        /* set up to act as client */
        struct addrinfo client_hints;
        memset(&client_hints, 0, sizeof(struct addrinfo));
        client_hints.ai_family = AF_INET;           /* IPv4 */
        client_hints.ai_socktype = SOCK_STREAM;     /* TCP */

        /* get addrinfo */
        struct addrinfo *server_res;
        status = getaddrinfo(hostname, target_port, &client_hints, &server_res);
        if (status != 0) {
            fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(status));
            printf("Killing thread: %d\n", data_in.thread_idx);
            return NULL;
        } 

        /* open remote socket */
        int server_fd = socket(server_res->ai_family, server_res->ai_socktype, server_res->ai_protocol);
        if (server_fd < 0) {
            fprintf(stderr, "error opening remote server socket: %d\n", errno);
            free(server_res);
            printf("Killing thread: %d\n", data_in.thread_idx);
            return NULL;
        } 

        /* connect to remote server */
        status = connect(server_fd, server_res->ai_addr, server_res->ai_addrlen);
        if (status < 0) {
            fprintf(stderr, "error connecting to remote server: %d\n", errno);
            free(server_res);
            close(server_fd);
            printf("Killing thread: %d\n", data_in.thread_idx);
            return NULL;
        }
        free(server_res);
        printf("connect success\n");

        /* Data pipeline loop */
        char outbound_request[MAXLINE];
        memset(outbound_request, 0, sizeof(outbound_request));
        snprintf(outbound_request, sizeof(outbound_request), "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", path_outbound, hostname);
        rio_writen(server_fd, outbound_request, strlen(outbound_request));

        /* initialize robust layer for remote server response */
        rio_t rio_server;
        rio_readinitb(&rio_server, server_fd);

        /* stream response back to browser */
        char server_buf[MAXLINE];
        while (1) {
            int bytes_from_web = rio_readn(&rio_server, server_buf, MAXLINE);
            if (bytes_from_web > 0) {
                rio_writen(client_fd, server_buf, bytes_from_web);
            } else if (bytes_from_web == 0) {
                /* EOF, remote server finished sending the website */
                break;
            } else {
            /* If the system call was just interrupted by the OS, try again! */
            if (errno == EINTR) {
                continue; 
            }
    
            /* For a genuine socket error, log it, close up shop, and keep the server alive */
            fprintf(stderr, "error reading website from remote server: %d\n", errno);
            break;
            }
        }

        close(server_fd);

    }
    else if (chars_read == 0) {
        printf("EOF received\n");
    }
    else {
        printf("Error reading from client\n");
    }   
    
    close(client_fd);
    printf("Closed client connection. Back to receive mode...\n");

    printf("Killing thread: %d\n", data_in.thread_idx);

    return NULL;

}
