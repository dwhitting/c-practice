#include "stan_hdr.h"

static bnk_acct_t *accts_ll = NULL;

static int delete_acct(void);
static int add_acct(void);
static bnk_acct_t *new_acct(void);
static void list_accts(void);
static int save_bnk_accts(void);
static int load_bnk_accts(void);
static int free_bnk_accts(void);
static int num_ll(void);

int bnk_acct_main(void) {

    printf("Size of struct: %lu\n", sizeof(bnk_acct_t));

    while (1) {
        printf("\nAccounts Menu:\n");
        printf("(a) add acct\n");
        printf("(d) delete acct\n");
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
        if (ch == 'l') {
            list_accts();
        }
        if (ch == 'o') {
            load_bnk_accts();
        }
        if (ch == 's') {
            save_bnk_accts();
        }
        if (ch == 'q') {
            break;
        }
    }

    return 0;
}

static int delete_acct(void) {
    
    int total_nodes = num_ll();
    if (total_nodes == 0) {
        printf("\nNo accts to remove\n");
        return 0;
    }

    list_accts();
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
        bnk_acct_t *head = accts_ll;
        accts_ll = accts_ll->next_acct;
        free(head);
        printf("\nRemoved first acct\n");
        return 0;
    } 
    
    bnk_acct_t *prev = NULL;
    bnk_acct_t *curr = accts_ll;

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
    bnk_acct_t *curr = accts_ll;
    while (curr != NULL) {
        cnt++;
        curr = curr->next_acct;
    }
    return cnt;
}

static bnk_acct_t *new_acct(void) {
    bnk_acct_t *new_a = (bnk_acct_t *)malloc(sizeof(bnk_acct_t));
    strcpy(new_a->name, "<no name>");
    new_a->balance = 0.0;
    new_a->next_acct = NULL;
    return new_a;
}

static void list_accts(void) {
    bnk_acct_t *curr = accts_ll;
    int idx = 1;
    printf("\n");
    while (curr != NULL) {
        printf("<%d> %-14s bal: $%.2f\n",idx++,  curr->name, curr->balance);
        curr = curr->next_acct;
    }
    printf("\n");
}

static int add_acct(void) {

    if (accts_ll == NULL) {
        accts_ll = new_acct();
        printf("First account...\n");
        printf("Enter acct name: ");
        char in_name[ACCT_NAME_LEN];
        fgets(in_name, ACCT_NAME_LEN, stdin);
        in_name[strcspn(in_name, "\n")] = '\0';
        strcpy(accts_ll->name, in_name); 
    } else {
        bnk_acct_t *curr = accts_ll;
        bnk_acct_t *head = accts_ll;
        while (curr->next_acct != NULL) {
            curr = curr->next_acct;
        }
        curr->next_acct = new_acct();
        printf("Enter acct name: ");
        char in_name[ACCT_NAME_LEN];
        fgets(in_name, ACCT_NAME_LEN, stdin);
        in_name[strcspn(in_name, "\n")] = '\0';
        strcpy(curr->next_acct->name, in_name);  
        accts_ll = head;
    }

    return 0;
}

static int save_bnk_accts(void) {
    int fd = open("bnk_data", O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd < 0) {
        stan_err("open bnk_data failed");
    }

    bnk_acct_t *curr = accts_ll;
    while (curr != NULL) {
        bnk_acct_t temp_save = *curr;

        write(fd, &temp_save, sizeof(bnk_acct_t));
        curr = curr->next_acct;
    }

    close(fd);

    return 0;
}

static int load_bnk_accts(void) {
    int fd = open("bnk_data", O_RDONLY);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("file does not exit...\n");
            return 0;
        } else {
            stan_err("open bnk_data failed");
            return -1;
        }
        
    }

    free_bnk_accts();
    bnk_acct_t *curr_node = NULL;
    ssize_t bytes_read;
    size_t struct_size = sizeof(bnk_acct_t);

    while (1) {
        bnk_acct_t *node_read = new_acct();
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
            printf("Partial struct read error in load_bnk_accts\n");
            free(node_read);
            break;
        }

        if (accts_ll == NULL) {
            accts_ll = node_read;
            curr_node = accts_ll;
        } else {
            curr_node->next_acct = node_read;
            curr_node = curr_node->next_acct;
        }

        curr_node->next_acct = NULL;

    }

    close(fd);

    return 0;

}

static int free_bnk_accts(void) {
    bnk_acct_t *prev = NULL;
    bnk_acct_t *curr = accts_ll;

    while (curr != NULL) {
        prev = curr;
        curr = curr->next_acct;
        free(prev);
    }
    
    accts_ll = NULL;

    return 0;
}

int bnk_accts_exit(void) {
    free_bnk_accts();
    return 0;
}