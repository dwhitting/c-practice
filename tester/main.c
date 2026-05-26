#include <stdio.h>

int some_func(void *in_buff) {
    char *buff;
    buff = in_buff;
    buff[0] = 'a';
    *++buff = 'b';
    *++buff = '\0';
    

    return 0;
}

int main(void)
{
    char buff[255];

    some_func(buff);

    printf("%s\n", buff);

    return 0;
}