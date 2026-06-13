#ifndef WRAP_H
#define WRAP_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int Pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
                    void *(start)(void *), void *vargp); 
int Pthread_join(pthread_t thread, void **retval);

#endif