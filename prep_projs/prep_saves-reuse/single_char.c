#include "stan_hdr.h"
#include <termios.h>
#include <errno.h>

int main(void) 
{
    struct termios newTerm, prevTerm;
    
    int fd = STDIN_FILENO, bytes_read;

    if (tcgetattr(fd, &newTerm) == -1)
        stan_err("tcgetattr fail");
    prevTerm = newTerm;

    newTerm.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(fd, TCSANOW, &newTerm);
    char ch;

    while (1) {
        bytes_read = read(STDIN_FILENO, &ch, 1);
        if (bytes_read == -1) {
            if (errno == EINTR) {
                printf("\nRead sig interupt. Resuming...\n");
                continue;
            } else {
                perror("read char error");
                break;
            }
        }
        if (bytes_read == 0) {
            printf("EOF detected...\n");
            break;
        }

        printf("pressed %c, ", ch);
        fflush(stdout);

        if (ch == 'q') {
            break;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &prevTerm);
    printf("\nterminal restored\n");

    return 0;
}
