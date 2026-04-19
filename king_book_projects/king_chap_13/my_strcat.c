#include <stdio.h>

char *my_strcat(char *s1, const char *s2);

int main(void) {

    char st1[30] = "first string ";
    char *st2 = "second string";
    my_strcat(st1, st2);
    printf("out: %s\n", st1);

    return 0;
}

char *my_strcat(char *s1, const char *s2) {
    char *p = s1;
    while (*p)
        p++;
    while (*p++ = *s2++)
        ;
    return s1;
}