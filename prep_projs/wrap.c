#include "wrap.h"

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