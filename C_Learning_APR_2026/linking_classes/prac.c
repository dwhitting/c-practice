#include "add_two_global.h"
#include <stdio.h>

int x = 1;
int y = 2;
int z;

int main(void)
{
    add_two();
    printf("%i + %i = %i\n", x, y, z);


    return 0;
}