#include <stdio.h>

int main(void)
{
    int ch, pos = 0;
    int line[50];
    while ((ch = getchar()) != 'x') {
        line[pos++] = ch;
        if (ch == '\n')
            pos--;
    }

    line[pos] = '\0';
    pos = 0;
    printf("\n");

    while (line[pos] != '\0') {
        printf("%c", line[pos++]);
    }

    printf("\n");

    return 0;
}