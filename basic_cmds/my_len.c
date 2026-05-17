#include <stdlib.h>

int my_len(const char *in_str) {
    int count = 0;

    if (in_str == NULL) return 0;
    while (in_str[count] != '\0') {
        count++;
    }

    return count;
}