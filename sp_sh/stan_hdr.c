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
    int num_links = num_ll(acct_type);
    if (num_links == 1 || num_links == 0) {
        return 0;
    }
    
    acct_t *curr = input_head;
    while (curr != NULL) {
        curr->date_sort = (100 * curr->month) + curr->day;
        curr = curr->next_acct;
    }

    /* set node that will be tested */
    acct_t *test;
    acct_t *test_minus_one;
    

    for (int j = num_links; j > 1; j--) {

        curr = input_head;
        test = NULL;
        test_minus_one = NULL;

        for (int i = 1; i < j; i++) {
            if (i == j - 1) {
                test = curr;
            } 
            if (i == j - 2) {
                test_minus_one = curr;
            }
            curr = curr->next_acct;
        }

        if (curr->date_sort < test->date_sort) {

            if (test_minus_one != NULL) {
                test_minus_one->next_acct = curr;
            } else {
                input_head = curr;
            }
       
            test->next_acct = curr->next_acct;    
            curr->next_acct = test;
        }

    }

    set_acct_head(acct_type, input_head);
    

    //printf("curr: %s, test: %s\n", curr->name, test->name);


    return 0;
}