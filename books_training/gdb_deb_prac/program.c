#include <stdio.h>

int add_two(int x, int y) {
    return x + y;
}

int main(void)
{
    int x = 5;
    int y = 3;
    int res = add_two(x, y);

    printf("result: %d\n", res);

    return 0;
}