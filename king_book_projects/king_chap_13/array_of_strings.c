#include <stdio.h>

int main(void) {
    char *planets[] = {"Mercury", "Venus", "Earth"};

    for (int i = 0; i < 3; i++) {
        printf("out: %s\n", planets[i]);
    }

    return 0;
}