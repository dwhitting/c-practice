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

char *month_to_str(Month in_month) {
    switch (in_month) {
        case None: return "Not Set";
        case JAN: return "JAN";
        case FEB: return "FEB";
        case MAR: return "MAR";
        case APR: return "APR";
        case MAY: return "MAY";
        case JUN: return "JUN";
        case JUL: return "JUL";
        case AUG: return "AUG";
        case SEP: return "SEP";
        case OCT: return "OCT";
        case NOV: return "NOV";
        case DEC: return "DEC";
        default: return "Unknown month entry";
    }
}

int sort_by_date(acct_t *input_head) {
    acct_type_t acct_type;
    acct_type.acct_Type = credAcct;
    if (num_ll(acct_type) == 1) {
        return 0;
    }
    
    acct_t *curr = input_head;
    while (curr != NULL) {
        curr->date_sort = (100 * curr->month) + curr->day;
        curr = curr->next_acct;
    }

    /* set node that will be tested */
    acct_t *test;
    for (int i = 0; i < 2; i++) {

    }



    return 0;
}