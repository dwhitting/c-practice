#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    char *fruit[] = {
        "watermelon",
        "banana",
        "pear",
        "apple"
    };

    for (int x = 0; x < 4; x++) {
        putchar(**(fruit+x));
        putchar('\n');
    }


    return 0;
}