#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("(a) for one\n");
    printf("(b) for two\n");
    printf("(c) for three\n");
    
    char c;
    
    while (c = getc(stdin)) {
        if (c == 'a')
            printf("one\n");
    }

    return 0;
}