#include <stdio.h>

int main(int argc, char *argv[]) {
    char *first = argv[1];
    char *second = argv[3];

    printf("%s and %s\n", second, first);

    return 0;
}