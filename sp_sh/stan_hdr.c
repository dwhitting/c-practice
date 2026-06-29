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

    /* zero or 1 elements already sorted */
    if (input_head == NULL || input_head->next_acct == NULL) {
        return 0; 
    }
    
    acct_t *curr = input_head;
    while (curr != NULL) {
        curr->date_sort = (100 * curr->month) + curr->day;
        curr = curr->next_acct;
    }

    /* use dummy node */
    acct_t dummy;
    dummy.next_acct = NULL;  /* acts as head of sorted list */
    
    curr = input_head;
    while (curr != NULL) {
        acct_t *next_node = curr->next_acct; // next node to process
        acct_t *prev = &dummy;
        while (prev->next_acct != NULL && prev->next_acct->date_sort < curr->date_sort) {
            prev = prev->next_acct;
        }

        curr->next_acct = prev->next_acct;
        prev->next_acct = curr;

        curr = next_node;
    }

    set_acct_head(acct_type, dummy.next_acct);

    return 0;
}

int move_acct_up_one(acct_type_t acct_type) {

    list_accts(acct_type);

    printf("Enter number to udpate: ");
    fflush(stdout);
    char ch = 'a';
    while (!isdigit(ch)) {
        ch = single_char_input();
    }
    int ud_line = ch - '0'; 

    int total_nodes = num_ll(acct_type);
    if (ud_line < 2 || ud_line > total_nodes) {
        printf("\nSelection out of range\n");
        return 0;
    }

    acct_t *curr = get_acct_head(acct_type);
    if (ud_line == 2) {
        acct_t *first = curr;
        acct_t *sec = curr->next_acct;
        acct_t *holder = sec->next_acct;
        sec->next_acct = first;
        first->next_acct = holder;
        set_acct_head(acct_type, sec);
    }

 

    return 0;
}