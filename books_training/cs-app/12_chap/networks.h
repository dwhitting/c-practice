#ifndef RIO_H
#define RIO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
//#include <arpa/inet.h>

#define RIO_BUFSIZE 8192
#define MAXLINE 8192
#define MAXBUF 8192
#define LISTENQ 1024
#define SA struct sockaddr

typedef struct {
    int rio_fd;                 /* descriptor for this internal buf */
    int rio_cnt;                /* unread bytes in internal buf */
    char *rio_bufptr;           /* next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE];  /* internal buffer */
} rio_t;

void unix_error(char *msg);
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
void Stat(const char *filename, struct stat *buf);
int open_clientfd(char *hostname, char *port);
int Open_listenfd(char *port);
void echo(int connfd);

#endif