#include <stdio.h>
#include <string.h>

int main(void)
{
    char s[50];

    while (1) {
        printf("Enter text: ");
        scanf("%[^\n]", s);
        getchar();
        printf("str: %s\n", s);
        printf("len: %lu\n", strlen(s));
        if (strlen(s) == 1 && *s == 'q') break;
    }

    

    return 0;
}