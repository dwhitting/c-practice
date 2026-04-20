#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int i;
    char c[20];
} my_struct;

int main(void)
{
    void *p = malloc(16);
    int *q = (int *)p;
    q[0] = 5;

    printf("%d\n", q[0]);

    return 0;
}



