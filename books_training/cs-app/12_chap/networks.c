#include "networks.h"

/* $begin unixerror */
void unix_error(char *msg) {        /* Unix-style error */

    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
/* $end unixerror */

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {

    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
	unix_error("Rio_readlineb error");
    return rc;
} 

ssize_t rio_readn(int fd, void *usrbuf, size_t n) {
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nread = read(fd, bufp, nleft)) < 0) {
            if (errno == EINTR) /* interrupted by sig handler return */
                nread = 0;      /* and call read() again */
            else
                return -1;      /* errno was set by the read, not a trap and sig handler */
        }
        else if (nread == 0)
            break;              /* EOF */
        nleft -= nread;
        bufp += nread;
    }
    return (n = nleft);         /* return >= 0 */
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n) {
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR)     /* interrupted by sig handler return */
                nwritten = 0;       /* and call write() again*/
            else
                return -1;          /* errno set my write() */
        }
    nleft -= nwritten;
    bufp += nwritten;
    }
    return n;   
}

void rio_readinitb(rio_t *rp, int fd) {
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {
    int cnt;

    while (rp->rio_cnt <= 0) { /* refill if buf is empty */
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0) {
            if (errno != EINTR) /* interrupted by sig handler return */
                return -1;
        }
        else if (rp->rio_cnt == 0) /* EOF */
            return 0;
        else 
            rp->rio_bufptr = rp->rio_buf; /* reset buffer ptr */
    }

    /* copy min(n, pr->rio_cnt) bytes from internal buf to user buf */
    cnt = n;
    if (rp->rio_cnt < n)
        cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
    int n, rc;
    char c, *bufp = usrbuf;

    for (n = 1; n < maxlen; n++) {
        if ((rc = rio_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n') {
                n++;
                break;
            }
        } else if (rc == 0) {
            if (n == 1)
                return 0;       /* EOF, no data read */
            else
                break;          /* EOF, some data base read*/
        } else
            return -1;          /* Error */
    }
    *bufp = 0;
    return n-1;
}

ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) {
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (n > 0) {
        if ((nread = rio_read(rp, bufp, nleft)) < 0)
            return -1;          /* errno set by read */
        else if (nread == 0)
            break;              /* EOF */
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);         /* return >= 0*/
}

void Stat(const char *filename, struct stat *buf) {

    if (stat(filename, buf) < 0)
	    unix_error("Stat error");
}

int open_clientfd(char *hostname, char *port) {
    int clientfd;
    struct addrinfo hints, *listp, *p;

    /* get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;        /* open a connection */
    hints.ai_flags = AI_NUMERICSERV;        /* ... using a numeric port arg */
    hints.ai_flags |= AI_ADDRCONFIG;        /* recommended for connections */
    getaddrinfo(hostname, port, &hints, &listp);

    /* walk the list for one that we can successfully connect to */
    for (p = listp; p; p = p->ai_next) {
        /* create socket descriptor */
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;   /* socket failed, try next */

        /* connect to server */
        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
            break;   /* success */
        close(clientfd);        /* connect failed, try another */
    }

    /* clean up */
    freeaddrinfo(listp);
    if (!p)     /* all connections failed */
        return -1;
    else
        return clientfd; 
}

int Open_listenfd(char *port) {
    struct addrinfo hints, *listp, *p;
    int listenfd, optval = 1;

    /* get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;                /* accept connections */
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;    /* ... on any IP address */
    hints.ai_flags |= AI_NUMERICSERV;               /* ... using port number */
    getaddrinfo(NULL, port, &hints, &listp);

    /* walk the list for one that we can bind to */
    for (p = listp; p; p = p->ai_next) {
        /* create socket descriptor */
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;       /* socket failed, try the next */

        /* eliminates "address already in use" error from bind, immediate listen on restart
         * instead of 30 sec delay */
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
                    (const void *)&optval, sizeof(int));

        /* bind the descriptor to the address */
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break;      /* success */
        close(listenfd);    /* bind failed, try the next */
    }

    /* clean up */
    freeaddrinfo(listp);
    if (!p)         /* no address worked */
        return -1;

    /* make it a listending socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0) {
        close(listenfd);
        return -1;
    }
    return listenfd;
}

void echo(int connfd) {
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    rio_readinitb(&rio, connfd);
    while ((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("server received %d bytes\n", (int)n);
        rio_writen(connfd, buf, n);
    }
}
