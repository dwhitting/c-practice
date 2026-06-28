#include "stan_hdr.h"

int stan_err(char * in_str) {
    perror(in_str);
    exit(1);
}

char single_char_input(void) 
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

        break;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &prevTerm);
    //printf("\nterminal restored\n");

    return ch;
}

void read_raw_line(char *buffer, size_t max_len) {

    fflush(stdout);
    
    ssize_t bytes_read = read(STDIN_FILENO, buffer, max_len -1);

    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        buffer[strcspn(buffer, "\n\r")] = '\0';
    } else {
        buffer[0] = '\0';
    }
}