#ifndef WRAP_H
#define WRAP_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int Pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
                    void *(start)(void *), void *vargp); 

#endif