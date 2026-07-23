#include "stan_hdr.h"

static record_t *long_term_record_ll = NULL;
static int save_records(void);
static int update_day_change(void);
static int list_records(void);
static float total_day_change(void);
static int update_note(void);
static int sort_reccs_by_date(void);
static int delete_record(void);
static int num_records(void);
static int select_record_line(void);
static int update_current_EOM(void);
static int update_record_date(void);
static int full_list_records(void);
//static int transfer_recs(void);

int records_menu(void) {

    while (1) {
        printf("\nRecords Main Menu:\n");
        printf("(l) List Record(s)\n");
        printf("(u) Update Current EOM\n");
        printf("(m) Update Month, Day, or Year\n");
        printf("(o) Load Record(s)\n");
        printf("(t) Update Note\n");
        printf("(s) Save Record(s)\n");
        printf("(d) Delete Record\n");
        printf("(f) Full Record List\n");

        printf("(q) Quit Records\n");
        printf("\nEnter selection: ");
        char ch = single_char_input();
        if (ch == 'f') {
            full_list_records();
        }
        if (ch == 'l') {
            list_records();
            printf("\nPress key to continue...");
            fflush(stdout);
            single_char_input();
        }
        if (ch == 'm') {
            update_record_date();
            sort_reccs_by_date();
        }
        if (ch == 'o') {
            load_records();
        }
        if (ch == 't') {
            update_note();
        }
        if (ch == 's') {
            save_records();
        }
        if (ch == 'u') {
            update_current_EOM();
        }
        if (ch == 'd') {
            delete_record();
        }
        if (ch == 'q') {
            break;
        }
    }

    return 0;
}

static int update_record_date(void) {

    list_records();

    char day_or_month;
    printf("(d) Day or (m) Month (y) Year: ");
    fflush(stdout);
    while (1) {
        day_or_month = single_char_input();
        if (day_or_month == 'd' || day_or_month == 'm' || day_or_month == 'y') {
            break;
        }
    }
    printf("char: %c\n", day_or_month);

    long ud_line = raw_read_long("Enter number to update: ");

    int total_nodes = num_records();
    if (ud_line < 1 || ud_line > total_nodes) {
        printf("\nSelection out of range\n");
        return 0;
    }

    int new_bal_i = raw_read_int("\nEnter new value: ");

    record_t *curr = long_term_record_ll;

    if (ud_line > 1) {
        for (int i = 1; i < ud_line; i++) {
            curr = curr->next_rec;
        }
    } 

    if (day_or_month == 'd') {
        curr->day = new_bal_i;
    } else if (day_or_month == 'm') {
        curr->month = new_bal_i;
    } else if (day_or_month == 'y') {
        curr->year = new_bal_i;
    }
    curr->date_sort = (10000 * curr->year) + (100 * curr->month) + curr->day; 

    printf("\n date sort: %d\n", curr->date_sort);

    printf("\nAcct updated\n");

    return 0;
}

int add_record(record_t *new_record) {
    record_t *record_head = long_term_record_ll;
    record_t *curr = record_head;

    strcpy(new_record->note, "<new rec added>");

    if (curr == NULL) {
        long_term_record_ll = new_record;
        update_day_change();
        return 0;
    }

    while (curr->next_rec != NULL) {
        curr = curr->next_rec;
    }

    curr->next_rec = new_record;
    update_day_change();

    return 0;
}

static int update_current_EOM(void) {

    int sel_line = select_record_line();
    
    if (sel_line == 0) {
        return 0;
    }

    record_t *curr = long_term_record_ll;
    for (int i = 1; i < sel_line; i++) {
        curr = curr->next_rec;
    }

    int new_bal_i = raw_read_int("\nEnter new value: ");
    curr->EOM_assets_minus_bills = new_bal_i;

    printf("Value updated\n");

    return 0;
}

static int select_record_line(void) {

    int total_nodes = num_records();
    if (total_nodes == 0) {
        printf("\nNo records to select\n");
        return 0;
    }

    list_records();
    int sel_line = raw_read_int("Enter selection: ");

    if (sel_line < 1 || sel_line > total_nodes) {
        printf("\nSelection out of range\n");
        return 0;
    }

    return sel_line;
}

static int delete_record(void) {

    int total_nodes = num_records();
    if (total_nodes == 0) {
        printf("\nNo accts to remove\n");
        return 0;
    }
    
    int rem_line = select_record_line();

    if (rem_line < 1 || rem_line > total_nodes) {
        printf("\nSelection out of range\n");
        return 0;
    }

    if (rem_line == 1) {
        record_t *old_head = long_term_record_ll;
        long_term_record_ll = old_head->next_rec;        
        free(old_head);
        printf("\nRemoved first acct\n");
        return 0;
    } 
    
    record_t *prev = NULL;
    record_t *curr = long_term_record_ll;

    for (int i = 1; i < rem_line; i++) {
        prev = curr;
        curr = curr->next_rec;
    }
    prev->next_rec = curr->next_rec;
    free(curr); 

    printf("\nAcct removed\n");

    return 0;
}

static int num_records(void) {
    int cnt = 0;
    record_t *curr = long_term_record_ll;

    if (curr == NULL) {
        return 0;
    }

    if (curr->next_rec == NULL) {
        return ++cnt;
    }

    while (curr != NULL) {
        cnt++;
        curr = curr->next_rec;
    }
    return cnt;
}

static int list_records(void) {

    sort_reccs_by_date();

    record_t *curr = long_term_record_ll;

    if (curr == NULL) {
        printf("\nlinked list is empty\n");
        return 0;
    }

    int idx = 1;

    while (curr != NULL) {
        char *mon = month_to_str(curr->month);
        printf("<%2d> %2d %s %4d Assets-CC: $%.2f EOM: $%.2f, $%.2f  Note: %s\n",idx++, curr->day, mon, curr->year,  
            (curr->assets_total - curr->cc_used_total), curr->EOM_assets_minus_bills, curr->day_change, curr->note); 
        
        curr = curr->next_rec;
    }       

    float total = total_day_change();
    char s_total[STR_NUM_LEN];
    float_to_currency(total, s_total);
    printf("\nTotal change: %s\n", s_total);        

    return 0;
}

static int full_list_records(void) {

    sort_reccs_by_date();

    record_t *curr = long_term_record_ll;

    if (curr == NULL) {
        printf("\nlinked list is empty\n");
        return 0;
    }

    int idx = 1;

    while (curr != NULL) {
        char *mon = month_to_str(curr->month);
        printf("<%2d> %2d %s %4d Days in Month: %d, Days to EOM: %d\n",idx++, curr->day, mon, curr->year, curr->days_in_month, curr->days_till_EOM);
        printf("Assets: %.2f, CC: %.2f, Inc: %.2f, RET Inc: %.2f\n", curr->assets_total, curr->cc_used_total, curr->income_total, curr->RET_income_total);
        printf("Bill: %.2f, Per d: %.2f, RET Per d: %.2f\n", curr->bills_total, curr->per_day, curr->RET_per_day);
        printf("EOM Assets-Bills: %.2f, day change: %.2f\n\n", curr->EOM_assets_minus_bills, curr->day_change);
        
        curr = curr->next_rec;
    }       

    float total = total_day_change();
    char s_total[STR_NUM_LEN];
    float_to_currency(total, s_total);
    printf("\nTotal change: %s\n", s_total);        

    return 0;
}

static int update_day_change(void) {
    record_t *records_head;
    records_head = long_term_record_ll;
    record_t *curr = records_head;

    if (records_head == NULL) {
        printf("*** Records linked list is NULL ***\n");
        return 0;
    }

    if (num_records() == 1) {
        curr->day_change = 0.0;
        curr->day -= 1;
        printf("Day %d\n", curr->day);
        return 0;
    }

    float prev_bal = 0.0;
    while (curr->next_rec != NULL) { 
        prev_bal = curr->EOM_assets_minus_bills - (curr->per_day * curr->days_till_EOM);
        curr = curr->next_rec;
    }
    curr->day_change = (curr->EOM_assets_minus_bills - (curr->per_day * curr->days_till_EOM)) - prev_bal;    

    return 0;
}

static float total_day_change(void) {

    record_t*curr = long_term_record_ll;
    float sum = 0.0;

    while (curr != NULL) {
        sum += curr->day_change;
        curr = curr->next_rec;
    }
    
    return sum;
}

static int sort_reccs_by_date(void) {

    record_t *input_head = long_term_record_ll;

    /* zero or 1 elements already sorted */
    if (input_head == NULL || input_head->next_rec == NULL) {
        return 0; 
    }
    
    record_t *curr = input_head;
    while (curr != NULL) {
        curr->date_sort = (10000 * curr->year) + (100 * curr->month) + curr->day;
        curr = curr->next_rec;
    }

    /* use dummy node */
    record_t dummy;
    dummy.next_rec = NULL;  /* acts as head of sorted list */
    
    curr = input_head;
    while (curr != NULL) {
        record_t *next_node = curr->next_rec; // next node to process
        record_t *prev = &dummy;
        while (prev->next_rec != NULL && prev->next_rec->date_sort < curr->date_sort) {
            prev = prev->next_rec;
        }

        curr->next_rec = prev->next_rec;
        prev->next_rec = curr;

        curr = next_node;
    }

    long_term_record_ll = dummy.next_rec;

    return 0;
}

int load_records(void) {

    int fd;
    char full_path[200];   /* since file is 100 */
    char file[ACCT_NAME_LEN];
    strcpy(file, "record_data");
    snprintf(full_path, sizeof(full_path), "%s/%s", DOC_PATH, file);

    fd = open(full_path, O_RDONLY);

    if (fd < 0) {
        if (errno == ENOENT) {
            printf("file does not exit...\n");
            return 0;
        } else {
            stan_err("open file data failed");
            return -1;
        }
    }

    free_records();
    record_t *head = NULL;
    record_t *curr_node = NULL;
    ssize_t bytes_read;

    while (1) {
        record_t *node_read = calloc(1, sizeof(record_t));
        if (node_read == NULL) {
            stan_err("error allocating node_read\n");

        }

        bytes_read = read(fd, &node_read->day, sizeof(int));
        if (bytes_read < 0) {
            printf("read error: %s\n", strerror(errno));
            free(node_read);
            break;
        }
        if (bytes_read == 0) {
            free(node_read);
            break;
        }

        read(fd, &node_read->month, sizeof(int));
        read(fd, &node_read->year, sizeof(int));
        read(fd, &node_read->date_sort, sizeof(int));
        read(fd, &node_read->days_in_month, sizeof(int));
        read(fd, &node_read->days_till_EOM, sizeof(int));
        read(fd, &node_read->assets_total, sizeof(float));
        read(fd, &node_read->cc_used_total, sizeof(float));
        read(fd, &node_read->income_total, sizeof(float));
        read(fd, &node_read->RET_income_total, sizeof(float));
        read(fd, &node_read->bills_total, sizeof(float));
        read(fd, &node_read->per_day, sizeof(float));
        read(fd, &node_read->RET_per_day, sizeof(float));
        read(fd, &node_read->EOM_assets_minus_bills, sizeof(float));
        read(fd, &node_read->day_change, sizeof(float));

        read(fd, &node_read->note, NOTE_LEN);

        node_read->next_rec = NULL;

        if (head == NULL) {
            head = node_read;
            curr_node = head;
        } else {
            curr_node->next_rec = node_read;
            curr_node = curr_node->next_rec;
        }

    }

    close(fd);
    long_term_record_ll = head;
    
    return 0;

}

int free_records(void) {
    record_t *head = long_term_record_ll;
    record_t *curr = head;
    record_t *next = NULL;

    while (curr != NULL) {
        next = curr->next_rec;
        free(curr);
        curr = next;
    }
    
    long_term_record_ll = NULL;
    return 0;
}

static int save_records(void) {

    sort_reccs_by_date();
    
    int fd;
    char full_path[200];     /* since file is 100 */
    char file[ACCT_NAME_LEN];
    strcpy(file, "record_data");
    snprintf(full_path, sizeof(full_path), "%s/%s", DOC_PATH, file);

    fd = open(full_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0) {
        stan_err("open file to save data failed");
    }

    record_t *curr = long_term_record_ll;
    while (curr != NULL) {
        record_t *temp_save = curr->next_rec;
        curr->next_rec = NULL;

        write(fd, &curr->day, sizeof(int));
        write(fd, &curr->month, sizeof(Month));
        write(fd, &curr->year, sizeof(int));
        write(fd, &curr->date_sort, sizeof(int));
        write(fd, &curr->days_in_month, sizeof(int));
        write(fd, &curr->days_till_EOM, sizeof(int));
        write(fd, &curr->assets_total, sizeof(float));
        write(fd, &curr->cc_used_total, sizeof(float));
        write(fd, &curr->income_total, sizeof(float));
        write(fd, &curr->RET_income_total, sizeof(float));
        write(fd, &curr->bills_total, sizeof(float));
        write(fd, &curr->per_day, sizeof(float));
        write(fd, &curr->RET_per_day, sizeof(float));
        write(fd, &curr->EOM_assets_minus_bills, sizeof(float));
        write(fd, &curr->day_change, sizeof(float));

        write(fd, &curr->note, NOTE_LEN);
    
        curr->next_rec = temp_save;
        curr = curr->next_rec;
    }

    close(fd);

    return 0;
}

static int update_note(void) {

    list_records();

    int ud_line = raw_read_int("Enter line number: ");
    char new_val_s[NOTE_LEN];

    raw_read_string("\nEnter note: ", new_val_s);

    record_t *curr = long_term_record_ll;
    for (int i = 1; i < ud_line; i++) {
        curr = curr->next_rec;
    }
    
    strcpy(curr->note, new_val_s);    

    return 0;
}

