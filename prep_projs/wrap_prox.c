#include "wrap_prox.h"

int Pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
                    void *(start)(void *), void *vargp) {
    int ret_val = pthread_create(thread, attr, start, vargp);
    if (ret_val != 0) {
        printf("thread create err: %d\n", ret_val);
        exit(1);
    }
    return ret_val;
}

int Pthread_join(pthread_t thread, void **retval) {
    int ret_val = pthread_join(thread, retval);
    if (ret_val != 0) {
        printf("thread join err: %d\n", ret_val);
        exit(1);
    }
    return ret_val;
}

ssize_t rio_write_n(int fd, char *buf, size_t n) {
    size_t nleft = n;
    ssize_t nwritten;
    char *buf_p = buf;

    while (nleft > 0) {
        if ((nwritten = write(fd, buf_p, nleft)) <= 0) {
            if (errno == EINTR)     /* SIGHNDLR return interupt */
                nwritten = 0;       /* and call write again */
            else
                return -1;          /* errno set by write */
        }
        nleft -= nwritten;
        buf_p += nwritten;
    }
    return n;
}

void rio_readinitb(rio_t *rp, int fd) {
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

/* this creates a buffer to read a big chunk from the network, then let
 * the byte-by-byte readline read from here, all in user mode.  Otherwise, 
 * you have to trap the kernel (context switch) for every byte read from
 * the network, which could lead to a massive performance hit. */
static ssize_t rio_read(rio_t *rp, char *usrbuf, ssize_t n) {
    ssize_t cnt;

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

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
    size_t n, rc;
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

void parse_url(char *url, char *host_name, char *tgt_port, char *path_outbound) {
    /* point to beginning of hostname */
    char *host_start = strstr(url, "//");
    if (host_start != NULL) {
        host_start += 2;
    } else {
        host_start = url;  /* there was no http://, so start at beginning of url */
    }

    /* locate path slash */
    char *path_start = strchr(host_start, '/');
    if (path_start != NULL) {
        strcpy(path_outbound, path_start);

        /* isolate host section */
        *path_start= '\0';
        //printf("hs: %s\n", host_start);

        /* isolate port */
        char *port_start = strchr(host_start, ':');
        if (port_start != NULL) {
            strcpy(tgt_port, port_start + 1);

            /* isolate host name */
            *port_start = '\0';
            strcpy(host_name, host_start);
        } else {
            strcpy(tgt_port, "80");     /* default http port if none is given */
            strcpy(host_name, host_start);
        }
    } else {
        /* Edge case if not trailing slash is provided */
        strcpy(path_outbound, "/");         /* default root path */

        /* still check if port colon exists in string */
        char *port_start = strchr(host_start, ':');
        if (port_start == NULL) {
            strcpy(tgt_port, port_start + 1);
            *port_start = '\0';
            strcpy(host_name, host_start);
        } else {
            strcpy(tgt_port, "80");         /* default http port */
            strcpy(host_name, host_start);
        }
    }
    
}