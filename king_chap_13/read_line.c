#include <stdio.h>

int read_line(char str[], int n);

int main(void) {
    char str[20];
    read_line(str, 20);
    printf("out: %s", str);

    return 0;
}

int read_line(char str[], int n) {
    int ch, i = 0;
    while ((ch = getchar()) != '\n')
        if (i < n)
            str[i++] = ch;
    str[i] = '\n';
    return i;
}