#include <unistd.h>
#include <errno.h>
#include "read_line.h" // book says to use this as a declaration for the function

ssize_t readLine(int fd, void *buffer, size_t n) {
    ssize_t numRead;    // num of bytes fetched by last read()
    size_t totRead;     // tot bytes read so far
    char *buf;
    char ch;

    if (n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    buf = buffer; // no pointer arithmatic on void * buffer

    totRead = 0;
    for (;;) {
        numRead = read(fd, &ch, 1);

        if (numRead == -1) {
            if (errno == EINTR)    // interupted, restart read()
                continue;
            else
                return -1;          // some other error

        } else if (numRead == 0) {  // EOF
            if (totRead == 0)       // no bytes read, return 0
                return 0;
            else                    // some bytes read add '\0'
                break;

        } else {                    // numRead must be 1 if we geet here
            if (totRead < n -1) {   // Discard > (n - 1) bytes
                totRead++;
                *buf++ = ch;
            }

            if (ch == '\n')
                break;
        }
    }

    *buf = '\0';
    return totRead;
    
}
