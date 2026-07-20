#include "stan_hdr.h"

static acct_t *bnk_accts_ll = NULL;
static acct_t *cc_accts_ll = NULL;
static acct_t *bill_accts_ll = NULL;
static acct_t *income_ll = NULL;
static acct_t *long_term_record_ll = NULL;

static int accts_menu(acct_type_t acct_type);
static int delete_acct(acct_type_t acct_type);
static int add_acct(acct_type_t acct_type);
static acct_t *new_acct(void);
static int save_accts(acct_type_t acct_type);
static int free_accts(acct_type_t acct_type);
static int update_balance(acct_type_t acct_type, char menu_sel);
static char *get_acct_type_name(acct_type_t acct_type);
static int update_cred_date(acct_type_t acct_type);
static int update_acct_name(acct_type_t acct_type);
static int print_accts_menu(acct_type_t acct_type);

int accts_main(void) {

    while (1) {
        printf("\nAccounts Main Menu:\n");
        printf("(b) Bank\n");
        printf("(c) Credit\n");
        printf("(q) quit\n");
        printf("\nEnter selection: ");
        acct_type_t acct_type;
        char ch = single_char_input();
        if (ch == 'b') {
            acct_type.acct_Type = bnkAcct;
            print_accts_menu(acct_type);
            accts_menu(acct_type);
        }
        if (ch == 'c') {
            acct_type.acct_Type = credAcct;
            print_accts_menu(acct_type);
            accts_menu(acct_type);
        }
        if (ch == 'q') {
            break;
        }
    }

    return 0;

}

static int print_accts_menu(acct_type_t acct_type) {

    int width = 25;

    printf("\n%s Update Menu:\n", get_acct_type_name(acct_type));
    printf("%-*s", width, "(a) add acct ");
    printf("(d) delete acct\n");
    if (acct_type.acct_Type == bnkAcct) {
        printf("%-*s", width, "(u) update balance ");
    }
    printf("(l) list accts\n");
    printf("%-*s", width, "(o) load bnk accts ");
    printf("(s) save\n");
    if (acct_type.acct_Type == credAcct) {
        printf("%-*s", width, "(i) update limit");
    }
    if (acct_type.acct_Type == credAcct) {
        printf("(r) update bal remain\n");
    }
    printf("(n) Update acct name\n");
    printf("%-*s", width, "(c) move row up one");
    printf("(q) quit accts\n");

    return 0;

}

static int accts_menu(acct_type_t acct_type) {
    
    while (1) {

        printf("\nEnter selection: ");
        fflush(stdout);
        char ch = single_char_input();

        if (ch == 'a') {
            add_acct(acct_type);
        }
        if (ch == 'd') {
            delete_acct(acct_type);
        }
        if (ch == 'u') {
            update_balance(acct_type, ch);
        }
        if ((ch == 'i' || ch == 'r') && acct_type.acct_Type == credAcct) {
            update_balance(acct_type, ch);
        }
        if (ch == 'l') {
            list_accts(acct_type);
            printf("\nPress key to continue...");
            fflush(stdout);
            single_char_input();
            print_accts_menu(acct_type);
        }
        if (ch == 'n') {
            update_acct_name(acct_type);
        }
        if (ch == 'o') {
            load_accts(acct_type);
        }
        if (ch == 's') {
            save_accts(acct_type);
        }
        if (ch == 'c') {
            move_acct_up_one(acct_type);
        }
        if (ch == 'q') {
            break;
        }
    }

    return 0;
}

int bills_menu(void) {

    acct_type_t acct_type;
    acct_type.acct_Type = billAcct;

    while (1) {
        printf("\nBills Main Menu:\n");
        printf("(a) Add bill\n");
        printf("(l) List bills\n");
        printf("(u) Update balance\n");
        printf("(m) Update month, day, or year\n");
        printf("(n) Update name\n");
        printf("(o) Load bills\n");
        printf("(s) Save bills\n");
        printf("(d) Delete bill\n");

        printf("(q) quit accts\n");
        printf("\nEnter selection: ");
        char ch = single_char_input();
        if (ch == 'a') {
            add_acct(acct_type);
        }
        if (ch == 'l') {
            list_accts(acct_type);
            printf("\nPress key to continue...");
            fflush(stdout);
            single_char_input();
            //print_accts_menu(acct_type);
        }
        if (ch == 'm') {
            update_cred_date(acct_type);
            sort_by_date(acct_type, get_acct_head(acct_type));
        }
        if (ch == 'n') {
            update_acct_name(acct_type);
        }
        if (ch == 'o') {
            load_accts(acct_type);
        }
        if (ch == 's') {
            save_accts(acct_type);
        }
        if (ch == 'u') {
            update_balance(acct_type, ch);
        }
        if (ch == 'd') {
            delete_acct(acct_type);
        }
        if (ch == 'q') {
            break;
        }
    }
    return 0;
}

int income_menu(void) {
    acct_type_t acct_type;
    acct_type.acct_Type = incomeAcct;

    while (1) {
        printf("\nIncome Main Menu:\n");
        printf("(a) Add Income\n");
        printf("(l) List Income(s)\n");
        printf("(u) Update balance\n");
        printf("(m) Update month, day, or year\n");
        printf("(n) Update name\n");
        printf("(o) Load Income(s)\n");
        printf("(s) Save Income(s)\n");
        printf("(d) Delete Income\n");

        printf("(q) quit Income\n");
        printf("\nEnter selection: ");
        char ch = single_char_input();
        if (ch == 'a') {
            add_acct(acct_type);
        }
        if (ch == 'l') {
            list_accts(acct_type);
            printf("\nPress key to continue...");
            fflush(stdout);
            single_char_input();
            //print_accts_menu(acct_type);
        }
        if (ch == 'm') {
            update_cred_date(acct_type);
            sort_by_date(acct_type, get_acct_head(acct_type));
        }
        if (ch == 'n') {
            update_acct_name(acct_type);
        }
        if (ch == 'o') {
            load_accts(acct_type);
        }
        if (ch == 's') {
            save_accts(acct_type);
        }
        if (ch == 'u') {
            update_balance(acct_type, ch);
        }
        if (ch == 'd') {
            delete_acct(acct_type);
        }
        if (ch == 'q') {
            break;
        }
    }

    return 0;
}

acct_t *get_acct_head(acct_type_t acct_type) {
    if (acct_type.acct_Type == bnkAcct) {
        return bnk_accts_ll;
    } else if (acct_type.acct_Type == credAcct) {
        return cc_accts_ll;
    } else if (acct_type.acct_Type == billAcct) {
        return bill_accts_ll;
    } else if (acct_type.acct_Type == incomeAcct) {
        return income_ll;
    } else if (acct_type.acct_Type == recordAcct) {
        return long_term_record_ll;
    } else {
        stan_err("acct in get_acct_head not recognized");
    }
    return NULL;
}

int set_acct_head(acct_type_t acct_type, acct_t *input_node) {
    if (acct_type.acct_Type == bnkAcct) {
        bnk_accts_ll = input_node;
    } else if (acct_type.acct_Type == credAcct) {
        cc_accts_ll = input_node;
    } else if (acct_type.acct_Type == billAcct) {
        bill_accts_ll = input_node;
    } else if (acct_type.acct_Type == incomeAcct) {
        income_ll = input_node;
    } else if (acct_type.acct_Type == recordAcct) {
        long_term_record_ll = input_node;
    } else {
        stan_err("acct type in set_acct_head not recognized");
    }
    return 0;
}

static char *get_acct_type_name(acct_type_t acct_type) {
    if (acct_type.acct_Type == bnkAcct) {
        return "Bank Account";
    } else if (acct_type.acct_Type == credAcct) {
        return "Credit Account";
    } else if (acct_type.acct_Type == billAcct) {
        return "Bill Account";
    } else if (acct_type.acct_Type == incomeAcct) {
        return "Income Account";
    } else {
        stan_err("acct type in acc_type_name not recognized");
    }
    return NULL;
}

static int update_acct_name(acct_type_t acct_type) {

    list_accts(acct_type);

    int ud_line = raw_read_int("Enter line number: ");
    char new_val_s[ACCT_NAME_LEN];
    raw_read_string("\nEnter new name: ", new_val_s);

    acct_t *curr = get_acct_head(acct_type);
    for (int i = 1; i < ud_line; i++) {
        curr = curr->next_acct;
    }
    
    strcpy(curr->name, new_val_s);
    
    return 0;
}

static int update_cred_date(acct_type_t acct_type) {

    list_accts(acct_type);

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

    int total_nodes = num_ll(acct_type);
    if (ud_line < 1 || ud_line > total_nodes) {
        printf("\nSelection out of range\n");
        return 0;
    }

    int new_bal_i = raw_read_int("\nEnter new value: ");

    acct_t *curr = get_acct_head(acct_type);

    if (ud_line > 1) {
        for (int i = 1; i < ud_line; i++) {
            curr = curr->next_acct;
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

static int update_balance(acct_type_t acct_type, char menu_sel) {

    int total_nodes = num_ll(acct_type);
    if (total_nodes == 0) {
        printf("\nNo accts to update\n");
        return 0;
    }

    list_accts(acct_type);

    int ud_line = raw_read_int("Enter number to update: ");

    if (ud_line < 1 || ud_line > total_nodes) {
        printf("\nSelection out of range\n");
        fflush(stdout);
        return 0;
    }

    float new_bal_f = raw_read_float("Enter new balance: ");

    acct_t *curr = get_acct_head(acct_type);

    if (ud_line > 1) {
        for (int i = 1; i < ud_line; i++) {
            curr = curr->next_acct;
        }
    } 

    if (menu_sel == 'u') {
        curr->balance = new_bal_f;
    } else if (menu_sel == 'i') {
        curr->cred_lim = new_bal_f;
        curr->balance = curr->cred_lim - curr->cred_remain;
    } else if (menu_sel == 'r') {
        curr->cred_remain = new_bal_f;
        curr->balance = curr->cred_lim - curr->cred_remain;
    }

    printf("\nAcct updated\n");

    return 0;
}

static int delete_acct(acct_type_t acct_type) {
    
    int total_nodes = num_ll(acct_type);
    if (total_nodes == 0) {
        printf("\nNo accts to remove\n");
        return 0;
    }

    list_accts(acct_type);
    int rem_line = raw_read_int("Enter number to remove: ");

    if (rem_line < 1 || rem_line > total_nodes) {
        printf("\nSelection out of range\n");
        return 0;
    }

    if (rem_line == 1) {
        acct_t *head = get_acct_head(acct_type);
        set_acct_head(acct_type, head->next_acct);
        free(head);
        printf("\nRemoved first acct\n");
        return 0;
    } 
    
    acct_t *prev = NULL;
    acct_t *curr = get_acct_head(acct_type);

    for (int i = 1; i < rem_line; i++) {
        prev = curr;
        curr = curr->next_acct;
    }
    prev->next_acct = curr->next_acct;
    free(curr); 

    printf("\nAcct removed\n");

    return 0;
}

int num_ll(acct_type_t acct_type) {
    int cnt = 0;
    acct_t *curr = get_acct_head(acct_type);
    while (curr != NULL) {
        cnt++;
        curr = curr->next_acct;
    }
    return cnt;
}

acct_t *get_new_acct(void) {
    return new_acct();
}

static acct_t *new_acct(void) {
    acct_t *new_a = (acct_t *)malloc(sizeof(acct_t));
    
    strcpy(new_a->name, "<no name>");
    new_a->balance = 0.0;
    new_a->cred_lim = 0.0;
    new_a->day = 0;
    new_a->month = 0;
    new_a->year = 0;
    new_a->cred_remain = 0.0;
    new_a->date_sort = 0;
    new_a->next_acct = NULL;
    return new_a;
}

int list_accts(acct_type_t acct_type) {

    acct_t *curr;
        
    if (acct_type.acct_Type == bnkAcct) {
        curr = bnk_accts_ll;
    } else if (acct_type.acct_Type == credAcct) {
        curr = cc_accts_ll;
    } else if (acct_type.acct_Type == billAcct) {
        curr = bill_accts_ll;
    } else if (acct_type.acct_Type == incomeAcct) {
        curr = income_ll;
    } 

    if (curr == NULL) {
        printf("\nlinked list is empty\n");
        return 0;
    }

    int idx = 1;
    printf("\n");
    while (curr != NULL) {
        if (acct_type.acct_Type == bnkAcct) {
            printf("<%d> %-14s bal: $%.2f\n",idx++,  curr->name, curr->balance);
        } else if (acct_type.acct_Type == credAcct) {
            curr->balance = curr->cred_lim - curr->cred_remain;
            printf("<%d> %-14s bal: $%.2f (remain: $%.2f)  <lim: $%.2f>\n",idx++, curr->name, 
                (curr->cred_lim - curr->cred_remain), curr->cred_remain, curr->cred_lim);
        } else if (acct_type.acct_Type == billAcct) {
            char *mon = month_to_str(curr->month);
            printf("<%2d> %2d %s %4d %-30s bal: $%.2f\n",idx++, curr->day, mon, curr->year, curr->name, 
                curr->balance);
        } else if (acct_type.acct_Type == incomeAcct) {
            char *mon = month_to_str(curr->month);
            printf("<%2d> %2d %s %4d %-30s bal: $%.2f\n",idx++, curr->day, mon, curr->year, curr->name, 
                curr->balance);      
        } 
        
        curr = curr->next_acct;
    }

    if (acct_type.acct_Type == billAcct) {
        float total = total_acct_balance(acct_type);
        char s_total[STR_NUM_LEN];
        float_to_currency(total, s_total);
        printf("\nTotal Bills: %s\n", s_total);
    }

    if (acct_type.acct_Type == credAcct) {
        float total = total_acct_balance(acct_type);
        char s_total[STR_NUM_LEN];
        float_to_currency(total, s_total);
        printf("\nTotal Bal: %s\n", s_total);
    }

    return 0;
}

static int add_acct(acct_type_t acct_type) {
    acct_t *curr = get_acct_head(acct_type);
    acct_t *head = curr;

    acct_t *today = (acct_t *)malloc(sizeof(acct_t));
    get_date(today);

    char in_name[ACCT_NAME_LEN];
    raw_read_string("\nEnter acct name: ", in_name);

    if (curr == NULL) {
        curr = new_acct();
        printf("First account...\n");
        strcpy(curr->name, in_name); 
        curr->day = today->day;
        curr->month = today->month;
        curr->year = today->year;
        curr->date_sort = (10000 * curr->year) + (100 * curr->month) + curr->day; 
        set_acct_head(acct_type, curr);
    } else {
        while (curr->next_acct != NULL) {
            curr = curr->next_acct;
        }
        curr->next_acct = new_acct();

        strcpy(curr->next_acct->name, in_name);
        curr->next_acct->day = today->day;
        curr->next_acct->month = today->month;
        curr->next_acct->year = today->year;
        curr->date_sort = (10000 * curr->year) + (100 * curr->month) + curr->day; 
        if (acct_type.acct_Type == bnkAcct) {
            bnk_accts_ll = head;
        } else if (acct_type.acct_Type == credAcct) {
            cc_accts_ll = head;
        } else if (acct_type.acct_Type == billAcct) {
            bill_accts_ll = head;
        } else if (acct_type.acct_Type == incomeAcct) {
            income_ll = head;
        } else if (acct_type.acct_Type == recordAcct) {
            long_term_record_ll = head;
        }
        
    }

    return 0;
}

static int save_accts(acct_type_t acct_type) {
    
    int fd;
    char full_path[200];     /* since file is 100 */
    char file[ACCT_NAME_LEN];
    if (acct_type.acct_Type == bnkAcct) {
        strcpy(file, "bnk_data");
    } else if (acct_type.acct_Type == credAcct) {
        strcpy(file, "cc_data");
    } else if (acct_type.acct_Type == billAcct) {
        strcpy(file, "bill_data");
    } else if (acct_type.acct_Type == incomeAcct) {
        strcpy(file, "income_data");
    } else if (acct_type.acct_Type == recordAcct) {
        strcpy(file, "record_data");
    }
    snprintf(full_path, sizeof(full_path), "%s/%s", DOC_PATH, file);

    fd = open(full_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0) {
        stan_err("open file to save data failed");
    }

    acct_t *curr = get_acct_head(acct_type);
    while (curr != NULL) {
        acct_t *temp_save = curr->next_acct;
        curr->next_acct = NULL;

        write(fd, curr->name, ACCT_NAME_LEN);
        write(fd, &curr->balance, sizeof(float));
        write(fd, &curr->cred_lim, sizeof(float));
        write(fd, &curr->day, sizeof(int));
        write(fd, &curr->month, sizeof(int));
        write(fd, &curr->year, sizeof(int));
        write(fd, &curr->date_sort, sizeof(int));
        write(fd, &curr->cred_remain, sizeof(float));
    
        curr->next_acct = temp_save;
        curr = curr->next_acct;
    }

    close(fd);

    return 0;
}

int load_all_accts(void) {
    acct_type_t acct_type;

    acct_type.acct_Type = bnkAcct;
    load_accts(acct_type);

    acct_type.acct_Type = credAcct;
    load_accts(acct_type);

    acct_type.acct_Type = billAcct;
    load_accts(acct_type);

    acct_type.acct_Type = incomeAcct;
    load_accts(acct_type);

    load_records();

    return 0;
}

int load_accts(acct_type_t acct_type) {

    int fd;
    char full_path[200];   /* since file is 100 */
    char file[ACCT_NAME_LEN];
    if (acct_type.acct_Type == bnkAcct) {
        strcpy(file, "bnk_data");
    } else if (acct_type.acct_Type == credAcct) {
        strcpy(file, "cc_data");      
    } else if (acct_type.acct_Type == billAcct) {
        strcpy(file, "bill_data");
    } else if (acct_type.acct_Type == incomeAcct) {
        strcpy(file, "income_data");
    } else if (acct_type.acct_Type == recordAcct) {
        strcpy(file, "record_data");
    }
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

    free_accts(acct_type);
    acct_t *head = get_acct_head(acct_type);
    acct_t *curr_node = NULL;
    ssize_t bytes_read;

    while (1) {
        acct_t *node_read = new_acct();
        if (node_read == NULL) {
            stan_err("error allocating node_read\n");
        }
        bytes_read = read(fd, node_read->name, ACCT_NAME_LEN);
        if (bytes_read == 0) {
            free(node_read);
            break;
        }

        if (bytes_read < ACCT_NAME_LEN) {
            printf("Data error while reading acct name\n");
            free(node_read);
            break;
        }

        read(fd, &node_read->balance, sizeof(float));
        read(fd, &node_read->cred_lim, sizeof(float));
        read(fd, &node_read->day, sizeof(int));
        read(fd, &node_read->month, sizeof(int));
        read(fd, &node_read->year, sizeof(int));
        read(fd, &node_read->date_sort, sizeof(int));
        read(fd, &node_read->cred_remain, sizeof(float));

        node_read->next_acct = NULL;

        if (head == NULL) {
            head = node_read;
            curr_node = head;
        } else {
            curr_node->next_acct = node_read;
            curr_node = curr_node->next_acct;
        }

    }

    close(fd);
    set_acct_head(acct_type, head);
    
    return 0;

}

static int free_accts(acct_type_t acct_type) {
    acct_t *head = get_acct_head(acct_type);
    acct_t *curr = head;
    acct_t *next = NULL;

    while (curr != NULL) {
        next = curr->next_acct;
        free(curr);
        curr = next;
    }
    
    set_acct_head(acct_type, NULL);
    return 0;
}

int accts_exit(void) {
    acct_type_t bnk = {.acct_Type = bnkAcct};
    acct_type_t cred = {.acct_Type = credAcct};
    acct_type_t bills = {.acct_Type = billAcct};
    acct_type_t income = {.acct_Type = incomeAcct};
    free_accts(bnk);
    free_accts(cred);
    free_accts(bills);
    free_accts(income);
    free_records();

    return 0;
}

float total_acct_balance(acct_type_t acct_type) {

    acct_t *curr = get_acct_head(acct_type);
    float sum = 0.0;

    while (curr != NULL) {
        sum += curr->balance;
        curr = curr->next_acct;
    }
    
    return sum;
}