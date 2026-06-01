#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#define MAXLINE 8192

int unix_error(char *msg);
pid_t Fork(void);
char *Fgets(char *ptr, int n, FILE *stream);

#endif
