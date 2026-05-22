#include <stdio.h>
#include <stdlib.h>
#include <gnu/libc-version.h>

typedef struct {
    int i;
    char c[20];
} my_struct;

int main(void)
{
    const char *val = gnu_get_libc_version();
    printf("%s\n", val);

    return 0;
}



