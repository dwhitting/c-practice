#include <sys/stat.h>
#include <fcntl.h>
#include "../headers/tlpi_hdr.h"

#ifndef BUF_SIZE        // allow "cc -D" to override definition
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[]) 
{
    int inputFd, outputFd, oopenFlags;
    mode_t filePers;
    ssize_t numRead;
    char buff[BUF_SIZE];

    if (argc != 3 || strcmp (argv[1], "--help") == 0)
        usageErr("%s old-file new-file\n", argv[0]);

    /* open input and output files */

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1)
        errExit("opening file %s", argv[1]);
    
    oopenFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePers = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
               S_IROTH |S_IWOTH;  // rw-rw-rw
    outputFd = open(argv[2], oopenFlags, filePers);
    if (outputFd == -1)
        errExit("opening file %s", argv[2]);

        // transfer data until we encounter end of input or an error

    while ((numRead = read(inputFd, buff, BUF_SIZE)) > 0)
        if (write(outputFd, buff, numRead) != numRead)
            fatal("write() returned error or partial write occurred");
    if (numRead == -1)
        errExit("read");

    if (close(inputFd) == -1)
        errExit("close input");
    if (close(outputFd) == -1)
        errExit("close output");
    
    exit(EXIT_SUCCESS);
}