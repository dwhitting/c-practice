#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
int main(void)
{
    int *p = NULL;

    int y = 0;
    //p = &y;
    
    //fprintf(stderr, "NULL pointer\n");
    assert(p != NULL);

    int x = *p;

    return 0;
}