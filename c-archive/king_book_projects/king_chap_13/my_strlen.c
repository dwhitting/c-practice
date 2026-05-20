#include <stdio.h>

size_t my_strlen(const char *s);

int main(void) {

    char *s = "test string";
    int res = my_strlen(s);
    printf("res: %d\n", res);

}

size_t my_strlen(const char *s) {
    const char *p = s;
    while (*s)
        s++;
    return s - p;
}