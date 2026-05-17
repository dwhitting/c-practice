#include "add_two_global.h"
#include <stdio.h>

extern int x, y, z;

void add_two(void)
{
   z = x + y;
}
