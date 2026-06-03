#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#define MAXLINE 8192
#define MAXBUF 8192

extern char **environ;

int unix_error(char *msg);
pid_t Fork(void);
char *Fgets(char *ptr, int n, FILE *stream);
void Kill(pid_t pid, int signum);
void *Malloc(size_t size);

#endif
