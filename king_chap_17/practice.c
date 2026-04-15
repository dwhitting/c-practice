#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *p;
    p = malloc(11);

    strcpy(p, "abc");

    printf("str out: %s\n", p);
    printf("%d\n", p[3]);
    

    return 0;
}