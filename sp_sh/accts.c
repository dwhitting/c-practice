#include "stan_hdr.h"

static acct_t *bnk_accts_ll = NULL;
static acct_t *cc_accts_ll = NULL;

static int accts_menu(acct_type_t acct_type);
static int delete_acct(acct_type_t acct_type);
static int add_acct(acct_type_t acct_type);
static acct_t *new_acct(void);
static int list_accts(acct_type_t acct_type);
static int save_accts(acct_type_t acct_type);
static int load_accts(acct_type_t acct_type);
static int free_accts(acct_type_t acct_type);
static int num_ll(acct_type_t acct_type);
static int update_balance(acct_type_t acct_type, char menu_sel);
static acct_t *get_acct_head(acct_type_t acct_type);
static int set_acct_head(acct_type_t acct_type, acct_t *input_node);
static char *get_acct_type_name(acct_type_t acct_type);
static int update_cred_date(acct_type_t acct_type);

static acct_t *get_acct_head(acct_type_t acct_type) {
    if (acct_type.acct_Type == bnkAcct) {
        return bnk_accts_ll;
    } else if (acct_type.acct_Type == credAcct) {
        return cc_accts_ll;
    } else {
        stan_err("acct in get_acct_head not recognized");
    }
    return NULL;
}

static int set_acct_head(acct_type_t acct_type, acct_t *input_node) {
    if (acct_type.acct_Type == bnkAcct) {
        bnk_accts_ll = input_node;
    } else if (acct_type.acct_Type == credAcct) {
        cc_accts_ll = input_node;
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
    } else {
        stan_err("acct type in acc_type_name not recognized");
    }
    return NULL;
}

int accts_main(void) {

    while (1) {
        printf("\nAccounts Main Menu:\n");
        printf("(b) bnk accts\n");
        printf("(c) cred accts\n");
        printf("(q) quit accts\n");
        acct_type_t acct_type;
        char ch = single_char_input();
        if (ch == 'b') {
            acct_type.acct_Type = bnkAcct;
            accts_menu(acct_type);
        }
        if (ch == 'c') {
            acct_type.acct_Type = credAcct;
            accts_menu(acct_type);
        }
        if (ch == 'q') {
            break;
        }
    }

    return 0;

}

static int accts_menu(acct_type_t acct_type) {
    while (1) {
        printf("\n%s Update Menu:\n", get_acct_type_name(acct_type));
        printf("(a) add acct\n");
        printf("(d) delete acct\n");
        printf("(u) update balance\n");
        if (acct_type.acct_Type == credAcct) {
            printf("(i) update limit\n");
        }
        if (acct_type.acct_Type == credAcct) {
            printf("(m) update day/month\n");
        }
        printf("(l) list accts\n");
        printf("(o) load bnk accts\n");
        printf("(s) save\n");
        printf("(q) quit accts\n");
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
        if (ch == 'i' && acct_type.acct_Type == credAcct) {
            update_balance(acct_type, ch);
        }
        if (ch == 'l') {
            list_accts(acct_type);
            printf("\nPress key to continue...");
            fflush(stdout);
            single_char_input();
        }
        if (ch == 'm' && acct_type.acct_Type == credAcct) {
            update_cred_date(acct_type);
        }
        if (ch == 'o') {
            load_accts(acct_type);
        }
        if (ch == 's') {
            save_accts(acct_type);
        }
        if (ch == 'q') {
            break;
        }
    }

    return 0;
}

static int update_cred_date(acct_type_t acct_type) {

    list_accts(acct_type);

    char day_or_month;
    printf("(d) Day or (m) Month: ");
    fflush(stdout);
    while (1) {
        day_or_month = single_char_input();
        if (day_or_month == 'd' || day_or_month == 'm') {
            break;
        }
    }
    printf("char: %c\n", day_or_month);

    printf("Enter number to udpate: ");
    fflush(stdout);
    char ch = 'a';
    while (!isdigit(ch)) {
        ch = single_char_input();
    }
    int ud_line = ch - '0'; 

    int total_nodes = num_ll(acct_type);
    if (ud_line < 1 || ud_line > total_nodes) {
        printf("\nSelection out of range\n");
        return 0;
    }

    printf("\nEnter new value: ");
    char new_val_s[ACCT_NAME_LEN];
    char *endptr;
    read_raw_line(new_val_s, ACCT_NAME_LEN);
    int new_bal_i = strtof(new_val_s, &endptr);
    if (new_val_s == endptr) {
        printf("No value entered\n");
        return 0;
    }

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
    }

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
    printf("Enter number to udpate: ");
    char ch = 'a';
    while (!isdigit(ch)) {
        ch = single_char_input();
    }
    int ud_line = ch - '0';

    if (ud_line < 1 || ud_line > total_nodes) {
        printf("\nSelection out of range\n");
        return 0;
    }

    printf("Enter new balance: ");
    char new_bal_s[ACCT_NAME_LEN];
    char *endptr;
    read_raw_line(new_bal_s, ACCT_NAME_LEN);
    float new_bal_f = strtof(new_bal_s, &endptr);
    if (new_bal_s == endptr) {
        printf("No value entered\n");
        return 0;
    }

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
    printf("Enter number to remove: ");
    char ch = 'a';
    while (!isdigit(ch)) {
        ch = single_char_input();
    }
    int rem_line = ch - '0';

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

static int num_ll(acct_type_t acct_type) {
    int cnt = 0;
    acct_t *curr = get_acct_head(acct_type);
    while (curr != NULL) {
        cnt++;
        curr = curr->next_acct;
    }
    return cnt;
}

static acct_t *new_acct(void) {
    acct_t *new_a = (acct_t *)malloc(sizeof(acct_t));
    strcpy(new_a->name, "<no name>");
    new_a->balance = 0.0;
    new_a->next_acct = NULL;
    return new_a;
}

static int list_accts(acct_type_t acct_type) {
    acct_t *curr;
    if (acct_type.acct_Type == bnkAcct) {
        curr = bnk_accts_ll;
    } else if (acct_type.acct_Type == credAcct) {
        curr = cc_accts_ll;
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
            char *mon = month_to_str(curr->month);
            printf("<%d> %2d %s %-14s bal: $%.2f  <lim: $%.2f>\n",idx++, curr->day, mon, curr->name, curr->balance, curr->cred_lim);
        }
        
        curr = curr->next_acct;
    }

    return 0;
}

static int add_acct(acct_type_t acct_type) {
    acct_t *curr = get_acct_head(acct_type);
    acct_t *head = curr;

    printf("Enter acct name: ");
    char in_name[ACCT_NAME_LEN];
    read_raw_line(in_name, ACCT_NAME_LEN);
    in_name[strcspn(in_name, "\n")] = '\0';

    if (curr == NULL) {
        curr = new_acct();
        printf("First account...\n");
        strcpy(curr->name, in_name); 
        set_acct_head(acct_type, curr);
    } else {
        while (curr->next_acct != NULL) {
            curr = curr->next_acct;
        }
        curr->next_acct = new_acct();

        strcpy(curr->next_acct->name, in_name);
        if (acct_type.acct_Type == bnkAcct) {
            bnk_accts_ll = head;
        } else {
            cc_accts_ll = head;
        }
        
    }

    return 0;
}

static int save_accts(acct_type_t acct_type) {
    
    int fd;
    if (acct_type.acct_Type == bnkAcct) {
        fd = open("bnk_data", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    } else if (acct_type.acct_Type == credAcct) {
        fd = open("cc_data", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }    
    if (fd < 0) {
        stan_err("open bnk_data failed");
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
    
        curr->next_acct = temp_save;
        curr = curr->next_acct;
    }

    close(fd);

    return 0;
}

static int load_accts(acct_type_t acct_type) {

    int fd;
    if (acct_type.acct_Type == bnkAcct) {
        fd = open("bnk_data", O_RDONLY);
    } else if (acct_type.acct_Type == credAcct) {
        fd = open("cc_data", O_RDONLY);
    }
    
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("file does not exit...\n");
            return 0;
        } else {
            stan_err("open bnk_data failed");
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
    free_accts(bnk);
    free_accts(cred);

    return 0;
}
