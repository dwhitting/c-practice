#include "stan_hdr.h"

typedef struct _record_t {
    float income_minus_bills;
    float EOM_assets_minus_per_days;
    float day_change;
    int days_til_EOM;
    float per_day;
    int day;
    Month month;
    int year;
    int date_sort;

    char note[NOTE_LEN];
    struct _record_t *next_rec;

} record_t;

static record_t *long_term_record_ll_new = NULL;
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
//static int transfer_recs(void);

int records_menu(void) {

    while (1) {
        printf("\nRecords Main Menu:\n");
        //printf("(a) Add Record\n");
        printf("(l) List Record(s)\n");
        printf("(u) Update Current EOM\n");
        printf("(m) Update Month, Day, or Year\n");
        printf("(o) Load Record(s)\n");
        printf("(t) Update Note\n");
        printf("(s) Save Record(s)\n");
        printf("(d) Delete Record\n");

        printf("(q) Quit Records\n");
        printf("\nEnter selection: ");
        char ch = single_char_input();
        // if (ch == 'a') {
        //     add_acct(acct_type);  CLASH WITH DISPLAY ADD REC
        // }
        if (ch == 'l') {
            list_records();
            printf("\nPress key to continue...");
            fflush(stdout);
            single_char_input();
        }
        // if (ch == 'm') {
        //     update_cred_date(acct_type);
        //     sort_by_date(acct_type, get_acct_head(acct_type));
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

static int update_current_EOM(void) {

    int sel_line = select_record_line();
    
    if (sel_line == 0) {
        return 0;
    }

    record_t *curr = long_term_record_ll_new;
    for (int i = 1; i < sel_line; i++) {
        curr = curr->next_rec;
    }

    int new_bal_i = raw_read_int("\nEnter new value: ");
    curr->income_minus_bills = new_bal_i;

    printf("Value updated\n");

    return 0;
}

int add_record(float EOM_assets_minus_bills, float per_day, 
        int days_til_EOM, float income_minus_bills) {
    record_t *record_head = long_term_record_ll_new;
    record_t *curr = record_head;
    record_t *new_record = calloc(1, sizeof(record_t));
    if (new_record == NULL) {
        printf("calloc failed in add record\n");
        return 1;
    }

    acct_t *acct_record = get_new_acct();
    get_date(acct_record);

    new_record->EOM_assets_minus_per_days = EOM_assets_minus_bills;
    new_record->income_minus_bills = income_minus_bills;
    new_record->per_day = per_day;
    new_record->days_til_EOM = days_til_EOM;
    new_record->day = acct_record->day;
    new_record->month = acct_record->month;
    new_record->year = acct_record->year;
    new_record->date_sort = acct_record->date_sort;
    strcpy(new_record->note, "<new rec added>");

    if (curr == NULL) {
        long_term_record_ll_new = new_record;
        return 0;
    }

    while (curr->next_rec != NULL) {
        curr = curr->next_rec;
    }

    curr->next_rec = new_record;

    return 0;
}

static int select_record_line(void) {

    int total_nodes = num_records();
    if (total_nodes == 0) {
        printf("\nNo records to select\n");
        return 0;
    }

    list_records();
    int sel_line = raw_read_int("Enter number to remove: ");

    if (sel_line < 1 || sel_line > total_nodes) {
        printf("\nSelection out of range\n");
        return 0;
    }

    return sel_line;
}

static int delete_record(void) {
    
    int rem_line = select_record_line();

    if (rem_line == 1) {
        record_t *head = long_term_record_ll_new;
        head = head->next_rec;
        free(head);
        printf("\nRemoved first acct\n");
        return 0;
    } 
    
    record_t *prev = NULL;
    record_t *curr = long_term_record_ll_new;

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
    record_t *curr = long_term_record_ll_new;
    while (curr != NULL) {
        cnt++;
        curr = curr->next_rec;
    }
    return cnt;
}

static int list_records(void) {

    update_day_change();
    sort_reccs_by_date();

    record_t *curr = long_term_record_ll_new;

    if (curr == NULL) {
        printf("\nlinked list is empty\n");
        return 0;
    }

    int idx = 1;

    while (curr != NULL) {
        char *mon = month_to_str(curr->month);
        printf("<%2d> %2d %s %4d Inc-Bills: $%.2f EOM: $%.2f, $%.2f  Note: %s\n",idx++, curr->day, mon, curr->year,  
            curr->income_minus_bills, curr->EOM_assets_minus_per_days, curr->day_change, curr->note); 
        
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
    records_head = long_term_record_ll_new;
    record_t *curr = records_head;

    if (records_head == NULL) {
        printf("*** Records linked list is NULL\n");
        return 0;
    }

    float prev_bal = 0.0;
    curr->day_change = 0.0;
    while (curr->next_rec != NULL) {
        prev_bal = curr->EOM_assets_minus_per_days;
        curr = curr->next_rec;
        curr->day_change = curr->EOM_assets_minus_per_days - prev_bal;
    }

    return 0;
}

static int sort_reccs_by_date(void) {

    record_t *input_head = long_term_record_ll_new;

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

    long_term_record_ll_new = dummy.next_rec;

    return 0;
}

static float total_day_change(void) {

    record_t*curr = long_term_record_ll_new;
    float sum = 0.0;

    while (curr != NULL) {
        sum += curr->day_change;
        curr = curr->next_rec;
    }
    
    return sum;
}

int load_records(void) {

    int fd;
    char full_path[200];   /* since file is 100 */
    char file[ACCT_NAME_LEN];
    strcpy(file, "record_data_new");
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

        bytes_read = read(fd, &node_read->income_minus_bills, sizeof(float));
        if (bytes_read < 0) {
            printf("read error: %s\n", strerror(errno));
            free(node_read);
            break;
        }
        if (bytes_read == 0) {
            free(node_read);
            break;
        }

        read(fd, &node_read->EOM_assets_minus_per_days, sizeof(float));
        read(fd, &node_read->days_til_EOM, sizeof(int));
        read(fd, &node_read->per_day, sizeof(float));
        read(fd, &node_read->day, sizeof(int));
        read(fd, &node_read->month, sizeof(Month));
        read(fd, &node_read->year, sizeof(int));
        read(fd, &node_read->date_sort, sizeof(int));

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
    long_term_record_ll_new = head;
    
    return 0;

}

int free_records(void) {
    record_t *head = long_term_record_ll_new;
    record_t *curr = head;
    record_t *next = NULL;

    while (curr != NULL) {
        next = curr->next_rec;
        free(curr);
        curr = next;
    }
    
    long_term_record_ll_new = NULL;
    return 0;
}

static int save_records(void) {

    sort_reccs_by_date();
    
    int fd;
    char full_path[200];     /* since file is 100 */
    char file[ACCT_NAME_LEN];
    strcpy(file, "record_data_new");
    snprintf(full_path, sizeof(full_path), "%s/%s", DOC_PATH, file);

    fd = open(full_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0) {
        stan_err("open file to save data failed");
    }

    record_t *curr = long_term_record_ll_new;
    while (curr != NULL) {
        record_t *temp_save = curr->next_rec;
        curr->next_rec = NULL;

        write(fd, &curr->income_minus_bills, sizeof(float));
        write(fd, &curr->EOM_assets_minus_per_days, sizeof(float));
        write(fd, &curr->days_til_EOM, sizeof(int));
        write(fd, &curr->per_day, sizeof(float));
        write(fd, &curr->day, sizeof(int));
        write(fd, &curr->month, sizeof(Month));
        write(fd, &curr->year, sizeof(int));
        write(fd, &curr->date_sort, sizeof(int));

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

    record_t *curr = long_term_record_ll_new;
    for (int i = 1; i < ud_line; i++) {
        curr = curr->next_rec;
    }
    
    strcpy(curr->note, new_val_s);    

    return 0;
}