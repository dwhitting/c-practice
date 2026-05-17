#include <stdio.h>

int my_get_char(void) {
    int ch = fgetc(stdin);

    if (ch == EOF) {
        if (ferror(stdin)) {
            // There was an error with I/O
            return EOF;
        }
        // Else, it was just the end of the file
    }
    return ch;
}