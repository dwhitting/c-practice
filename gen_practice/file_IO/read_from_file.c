#include <stdio.h>
#include <stdlib.h>

int read_from_file(void) {
    FILE *fh;
    int ch;

    fh=fopen("hello.txt", "r");
    if (fh == NULL) {
        puts("Can't open file.\n");
        return 1;
    }

    while (!feof(fh)) {
        ch = fgetc(fh);
        if (ch == EOF) {
            break;
        }
        putchar(ch);
    }
    fclose(fh);
    
    return 0;
}