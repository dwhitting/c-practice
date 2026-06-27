#include "stan_hdr.h"

static acct_t *bnk_accts_ll = NULL;
static acct_t *cc_accts_ll = NULL;

static int accts_menu(acct_type_t acct_type);
static int delete_acct(void);
static int add_acct(void);
static acct_t *new_acct(void);
static void list_accts(acct_type_t acct_type);
static int save_accts(void);
static int load_accts(acct_type_t acct_type);
static int free_accts(void);
static int num_ll(void);
static int update_balance(void);

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
            break;
        }
        if (ch == 'q') {
            break;
        }
    }

    return 0;

}

static int accts_menu(acct_type_t acct_type) {
    while (1) {
        printf("\nUpdate Menu:\n");
        printf("(a) add acct\n");
        printf("(d) delete acct\n");
        printf("(u) update balance\n");
        printf("(l) list accts\n");
        printf("(o) load bnk accts\n");
        printf("(s) save\n");
        printf("(q) quit accts\n");
        char ch = single_char_input();
        if (ch == 'a') {
            add_acct();
        }
        if (ch == 'd') {
            delete_acct();
        }
        if (ch == 'u') {
            update_balance();
        }
        if (ch == 'l') {
            list_accts(acct_type);
        }
        if (ch == 'o') {
            load_accts(acct_type);
        }
        if (ch == 's') {
            save_accts();
        }
        if (ch == 'q') {
            break;
        }
    }

    return 0;
}

static int update_balance(void) {

    int total_nodes = num_ll();
    if (total_nodes == 0) {
        printf("\nNo accts to update\n");
        return 0;
    }

    //list_accts();
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
    fgets(new_bal_s, ACCT_NAME_LEN, stdin);
    float new_bal_f = strtof(new_bal_s, &endptr);
    if (new_bal_s == endptr) {
        printf("No value entered\n");
        return 0;
    }

    if (ud_line == 1) {
        bnk_accts_ll->balance = new_bal_f;
        printf("Ammount updated\n");
        return 0;
    } 

    acct_t *curr = bnk_accts_ll;

    for (int i = 1; i < ud_line; i++) {
        curr = curr->next_acct;
    }
    curr->balance = new_bal_f;

    printf("\nAcct updated\n");

    return 0;
}

static int delete_acct(void) {
    
    int total_nodes = num_ll();
    if (total_nodes == 0) {
        printf("\nNo accts to remove\n");
        return 0;
    }

    //list_accts();
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
        acct_t *head = bnk_accts_ll;
        bnk_accts_ll = bnk_accts_ll->next_acct;
        free(head);
        printf("\nRemoved first acct\n");
        return 0;
    } 
    
    acct_t *prev = NULL;
    acct_t *curr = bnk_accts_ll;

    for (int i = 1; i < rem_line; i++) {
        prev = curr;
        curr = curr->next_acct;
    }
    prev->next_acct = curr->next_acct;
    free(curr); 

    printf("\nAcct removed\n");

    return 0;
}

static int num_ll(void) {
    int cnt = 0;
    acct_t *curr = bnk_accts_ll;
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

static void list_accts(acct_type_t acct_type) {
    acct_t *curr;
    if (acct_type.acct_Type == bnkAcct) {
        curr = bnk_accts_ll;
    }
    int idx = 1;
    printf("\n");
    while (curr != NULL) {
        printf("<%d> %-14s bal: $%.2f\n",idx++,  curr->name, curr->balance);
        curr = curr->next_acct;
    }
    printf("\n");
}

static int add_acct(void) {

    if (bnk_accts_ll == NULL) {
        bnk_accts_ll = new_acct();
        printf("First account...\n");
        printf("Enter acct name: ");
        char in_name[ACCT_NAME_LEN];
        fgets(in_name, ACCT_NAME_LEN, stdin);
        in_name[strcspn(in_name, "\n")] = '\0';
        strcpy(bnk_accts_ll->name, in_name); 
    } else {
        acct_t *curr = bnk_accts_ll;
        acct_t *head = bnk_accts_ll;
        while (curr->next_acct != NULL) {
            curr = curr->next_acct;
        }
        curr->next_acct = new_acct();
        printf("Enter acct name: ");
        char in_name[ACCT_NAME_LEN];
        fgets(in_name, ACCT_NAME_LEN, stdin);
        in_name[strcspn(in_name, "\n")] = '\0';
        strcpy(curr->next_acct->name, in_name);  
        bnk_accts_ll = head;
    }

    return 0;
}

static int save_accts(void) {
    int fd = open("bnk_data", O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd < 0) {
        stan_err("open bnk_data failed");
    }

    acct_t *curr = bnk_accts_ll;
    while (curr != NULL) {
        acct_t temp_save = *curr;

        write(fd, &temp_save, sizeof(acct_t));
        curr = curr->next_acct;
    }

    close(fd);

    return 0;
}

static int load_accts(acct_type_t acct_type) {

    int fd;
    if (acct_type.acct_Type == bnkAcct) {
        fd = open("bnk_data", O_RDONLY);
    } else {
        return 0;
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

    free_accts();
    acct_t *curr_node = NULL;
    ssize_t bytes_read;
    size_t struct_size = sizeof(acct_t);

    while (1) {
        acct_t *node_read = new_acct();
        bytes_read = read(fd, node_read, struct_size);
        if (node_read == NULL) {
            stan_err("error allocating node_read\n");
        }

        if (bytes_read == 0) {
            free(node_read);
            break;
        }

        if (bytes_read < 0) {
            free(node_read);
            stan_err("Error while reading bnk_accts file");
        }

        if (bytes_read < (ssize_t)struct_size) {
            printf("Partial struct read error in load_accts\n");
            free(node_read);
            break;
        }

        if (bnk_accts_ll == NULL) {
            bnk_accts_ll = node_read;
            curr_node = bnk_accts_ll;
        } else {
            curr_node->next_acct = node_read;
            curr_node = curr_node->next_acct;
        }

        curr_node->next_acct = NULL;

    }

    close(fd);

    return 0;

}

static int free_accts(void) {
    acct_t *prev = NULL;
    acct_t *curr = bnk_accts_ll;

    while (curr != NULL) {
        prev = curr;
        curr = curr->next_acct;
        free(prev);
    }
    
    bnk_accts_ll = NULL;

    return 0;
}

int accts_exit(void) {
    free_accts();
    return 0;
}