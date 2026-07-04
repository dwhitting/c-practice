#include "stan_hdr.h"

int stan_err(char * in_str) {
    perror(in_str);
    exit(1);
}

char single_char_input(void) 
{
    fflush(stdout);
    
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
    printf("%c\n", ch);

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

    printf("Enter number to update: ");
    char new_val_s[ACCT_NAME_LEN];
    char *endptr;
    read_raw_line(new_val_s, ACCT_NAME_LEN);
    int new_bal_i = strtol(new_val_s, &endptr, 10);
    if (new_val_s == endptr) {
        printf("No value entered\n");
        return 0;
    }
    int ud_line = new_bal_i;

    int total_nodes = num_ll(acct_type);
    if (ud_line < 2 || ud_line > total_nodes) {
        printf("\nSelection out of range\n");
        return 0;
    }

    acct_t *head = get_acct_head(acct_type);

    if (ud_line == 2) {
        acct_t *first = head;
        acct_t *second = head->next_acct;

        first->next_acct = second->next_acct;
        second->next_acct = first;

        set_acct_head(acct_type, second);
    } else {

        acct_t *before = head;
        for (int i = 1; i < ud_line - 2; i++) {
            before = before->next_acct; // Node at ud_liine -2
        }

        acct_t *prev = before->next_acct;   // Node at ud_line -1
        acct_t *target = prev->next_acct;   // Node at ud_line
        acct_t *after = target->next_acct;  // Node after ud_line
        
        /* Swap */
        before->next_acct = target;
        target->next_acct = prev;
        prev->next_acct = after;
        
        /* head hasn't changed so not calling set_acct_head */
    }

    return 0;
}

long raw_read_long(char *prompt) {

    printf("%s", prompt);
    fflush(stdout);
    char ud_s[ACCT_NAME_LEN];
    char *endptr;
    read_raw_line(ud_s, ACCT_NAME_LEN);
    long ret_long = strtol(ud_s, &endptr, 10);

    if (ud_s == endptr) {
        printf("No value entered\n");
        return 0;
    }

    return ret_long;
}

int raw_read_int(char *prompt) {

    printf("%s", prompt);
    fflush(stdout);
    char new_val_s[ACCT_NAME_LEN];
    char *endptr;
    read_raw_line(new_val_s, ACCT_NAME_LEN);
    int new_bal_i = strtol(new_val_s, &endptr, 10);
    if (new_val_s == endptr) {
        printf("No value entered\n");
        return 0;
    }

    return new_bal_i;
}

int raw_read_string(char *prompt, char *ret_string) {

    printf("%s", prompt);
    fflush(stdout);
    char new_val_s[ACCT_NAME_LEN];
    read_raw_line(new_val_s, ACCT_NAME_LEN);
    if (strlen(new_val_s) == 0) {
        printf("No value entered\n");
        return 0;
    }

    strcpy(ret_string, new_val_s);

    return 0;

}

float raw_read_float(char *prompt) {

    printf("%s", prompt);
    fflush(stdout);
    char new_bal_s[ACCT_NAME_LEN];
    char *endptr;
    read_raw_line(new_bal_s, ACCT_NAME_LEN);
    float new_bal_f = strtof(new_bal_s, &endptr);
    if (new_bal_s == endptr) {
        printf("No value entered\n");
        return 0;
    }

    return new_bal_f;
}

int get_date(acct_t *ret_date) {
    time_t raw_time;
    struct tm *local_time;
    
    /* get epoch time */
    time(&raw_time);

    /* convert to local time struct */
    local_time = localtime(&raw_time);

    int day = local_time->tm_mday;
    int month = local_time->tm_mon + 1;     // offset 0-11 to 1-12
    int year = local_time->tm_year + 1900;  // offset since 1990

    ret_date->day = day;
    ret_date->month = month;
    ret_date->year = year;

    if (acct_type.acct_Type == credAcct) {
        acct_t *today = malloc(sizeof(acct_t));
        get_date(today);

        strcpy(today->name, "TODAY");
        curr_node->next_acct = today;

        //printf("The curr: %s\n", curr_node->name);
    }

    /* format as date */
    //char date_string[50];
    //strftime(date_string, sizeof(date_string), "%m-%d-%Y", local_time);
    
    return 0;
}