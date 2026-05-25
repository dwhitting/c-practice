#include <stdio.h>
#include "IO_headers.h"
#include <string.h>

int main(int argc, char *argv[])
{

    if (argc < 2) {
        printf("there needs to be a desired function listed\n");
        return -1;
    }

    if (strcmp(argv[1], "copy_file") == 0) {
        copy_file(argc, argv);
    }

    

    return 0;
}