#define _GNU_SOURCE    /* needed this for intellisence to see addrinfo type */
#ifndef WRAP_PROX_H
#define WRAP_PROX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>  // Definitions for data types (like size_t, ssize_t)
#include <sys/socket.h> // Core socket definitions, structures, and functions
#include <netdb.h>      // For getaddrinfo, struct addrinfo, and gai_strerror
#define MAXLINE 8192
#define LISTENQ 1024

typedef struct {
    int rio_fd;                 /* descriptor for this internal buf */
    ssize_t rio_cnt;                /* unread bytes in internal buf */
    char *rio_bufptr;           /* next unread byte in internal buf */
    char rio_buf[MAXLINE];  /* internal buffer */
} rio_t;

int Pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
                    void *(start)(void *), void *vargp); 
int Pthread_join(pthread_t thread, void **retval);
ssize_t net_write_n(int fd, char *buf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
static ssize_t rio_read(rio_t *rp, char *usrbuf, ssize_t n);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

#endif