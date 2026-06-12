#include <stdio.h>

size_t my_strlen(const char *s);
char *my_strcat(char *s1, const char *s2);

int main(void) {

    char s1[30] = "first ";
    char *s2 = "second string";

    char *ret = my_strcat(s1, s2);

    printf("%s\n", s1);
    //printf("s1 len: %d\n", (int) my_strlen(s1));

    return 0;
}

char *my_strcat(char *s1, const char *s2) {
    char *p = s1;
    while (*p)
        p++;
    while (*p++ = *s2++) {
        ;
    }
    return s1;
}

size_t my_strlen(const char *s) {
    size_t n = 0;
    while (*s++)
        n++;
    return n;
}