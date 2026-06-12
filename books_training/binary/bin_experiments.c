#include <stdio.h>

int tester(void) {

    int *pnum;
    int num = 97;
    pnum = &num;

    printf("char %lu\n", (unsigned long)*pnum);

    return 0;
}