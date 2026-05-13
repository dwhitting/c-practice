#include <stdlib.h>

int main(void)
{
    int *malloc_error = (int *) malloc(sizeof(int));
    *malloc_error = 5;

    return 0;
}