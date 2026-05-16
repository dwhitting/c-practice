#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    void *ptr = NULL;

    int x = 5;

    ptr = &x;

    printf("res: %d\n", *(int *) ptr);

    char ch = 'c';
    ptr = &ch;

    printf("char: %c\n", *(char *) ptr);



    return 0;
}