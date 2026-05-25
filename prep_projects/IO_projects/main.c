#include <stdio.h>
#include "IO_headers.h"
#include <string.h>

int main(int argc, char *argv[])
{
    // This project is to imitate some file functions.
    // the -o file is my_io, so to copy files, use the
    // command ./my_io copy_file [file 1] [file 2]

    if (argc < 2) {
        printf("there needs to be a desired function listed\n");
        return -1;
    }

    if (strcmp(argv[1], "copy_file") == 0) {
        copy_file(argc, argv);
    }

    

    return 0;
}