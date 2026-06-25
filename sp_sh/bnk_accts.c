#include "stan_hdr.h"

static bnk_acct_t *accts_ll;

static int add_acct(void);
static bnk_acct_t *new_acct(void);
static void list_accts(void);
static int save_bnk_accts(void);

static int init_accts_ll(void) {
    accts_ll = new_acct();
    return 0;
}

static bnk_acct_t *new_acct(void) {
    bnk_acct_t *new_a = (bnk_acct_t *)malloc(sizeof(bnk_acct_t));
    strcpy(new_a->name, "<no name>");
    new_a->balance = 0.0;
    new_a->next_acct = NULL;
    return new_a;
}


int bnk_acct_main(void) {
    init_accts_ll();
    while (1) {
        printf("\nAccounts Menu:\n");
        printf("(a) add acct\n");
        printf("(l) list accts\n");
        printf("(s) save\n");
        printf("(q) quit accts\n");
        char ch = single_char_input();
        if (ch == 'a') {
            add_acct();
        }
        if (ch == 'l') {
            list_accts();
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

static void list_accts(void) {
    bnk_acct_t *curr = accts_ll;
    printf("\n");
    while (curr != NULL) {
        printf("Acct: %s, bal: $%.2f\n", curr->name, curr->balance);
        curr = curr->next_acct;
    }
    printf("\n");
}

static int add_acct(void) {
    
    bnk_acct_t *curr = accts_ll;
    while (curr->next_acct != NULL) {
        curr = curr->next_acct;
    }
    if (strcmp(accts_ll->name, "<no name>") == 0) {
        printf("First account...\n");
        printf("Enter acct name: ");
        char in_name[ACCT_NAME_LEN];
        fgets(in_name, ACCT_NAME_LEN, stdin);
        in_name[strcspn(in_name, "\n")] = '\0';
        strcpy(accts_ll->name, in_name); 
    } else {
        bnk_acct_t *new_node = new_acct();
        printf("Enter acct name: ");
        char in_name[ACCT_NAME_LEN];
        fgets(in_name, ACCT_NAME_LEN, stdin);
        in_name[strcspn(in_name, "\n")] = '\0';
        strcpy(new_node->name, in_name);     
        curr->next_acct = new_node;
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
        temp_save.next_acct = NULL;

        write(fd, &temp_save, sizeof(bnk_acct_t));
        curr = curr->next_acct;
    }

    close(fd);

    return 0;
}

int load_bnk_accts(void) {
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

    accts_ll = new_acct();
    bnk_acct_t *curr_node = accts_ll;
    ssize_t bytes_read;
    size_t struct_size = sizeof(bnk_acct_t);

    while (1) {
        bnk_acct_t *node_read = new_acct();
        if (node_read == NULL) {
            stan_err("error allocating node_read\n");
        }

        bytes_read = read(fd, node_read, struct_size);
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
        node_read->next_acct = NULL;

        if (strcmp(accts_ll->name, "<no name>") == 0) {
            strcpy(accts_ll->name, node_read->name);
            accts_ll->balance = node_read->balance;
            free(node_read);
            curr_node = accts_ll;
        } else {
            curr_node->next_acct = node_read;
            curr_node = node_read;
        }
    }

    close(fd);

    return 0;

}