#include <stdio.h>

int main(void)
{
    char c = '\n';
    printf("res: %x\n", c);

    int *p;
    p = NULL;
    printf("res2: %d\n", *p);

    return 0;
}