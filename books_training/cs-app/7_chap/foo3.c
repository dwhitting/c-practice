#include <stdio.h>

void f(void);
int f_1(void);
int x = 15210;

int main(void)
{
    f();
    printf("x = %d\n", x);
    printf("funcs: %d\n", f_1());

    return 0;
}