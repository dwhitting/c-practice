#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("location of code: %p\n", (void *) main);
    void *mptr = malloc(1);
    printf("location of heap: %p\n", mptr);
    int x = 1;
    printf("location of stack: %p\n", (void *) &x);

    free(mptr);
    return 0;
}