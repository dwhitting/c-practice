#include<stdio.h>
#include <stdlib.h>

int write_to_file(void)
{
    FILE *fh;

    // open file
    fh=fopen("hello.txt", "a");
    // check for errors
    if (fh == NULL) {
        puts("Can't open file\n");
        return 1;
    }
    // output text
    fprintf(fh, "This is put in the file.\n");

    fclose(fh);

    puts("File written.\n");

    return 0;
}