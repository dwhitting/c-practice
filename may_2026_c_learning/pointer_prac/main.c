#include <stdlib.h>
#include <stdio.h>

void *void_f(void *arg) {
    int val = *(int *) arg;
    val += 5;
    void *ret = &val;
    return ret; 
}

int main(void)
{
    int x = 7;
    void *p = &x;
    void *f_ptr = void_f(p);

    printf("ret val: %d\n", *(int *)f_ptr);

    return 0;
}