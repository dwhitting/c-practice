#include <stdio.h>
#include <limits.h>

int main(void)
{
    int x = 5;

    printf("x bin: %.4b\n", x);
    printf("~x bin: %.4b\n", ~x);
    printf("\n");

    short int v = -12345;
    unsigned short uv = (unsigned short) v;
    printf("v= %d, uv= %u\n", v, uv);

    
    size_t st = 1000;
    int i_st = (int) st;
    printf("i_st: %d\n\n", i_st);

    char *byte_v = &st;
    for (int i = 0; i < sizeof(size_t); i++) {
        printf(" %x", byte_v[i]);
    }
    printf("\n");

    return 0;
}