#include "stan_hdr.h"

static int add_acct(bnk_acct_t *list_head);

int bnk_acct_main(bnk_acct_t *list_head) {

    while (1) {
        printf("\nAccounts Menu:\n");
        printf("(a) add acct\n");
        printf("(q) quit accts\n");
        char ch = single_char_input();
        if (ch == 'a') {
            add_acct(list_head);
        }
        if (ch == 'q') {
            break;
        }
    }



    return 0;
}

static int add_acct(bnk_acct_t *list_head) {
    bnk_acct_t *curr_node = list_head;
    while (list_head == NULL || list_head->next_acct != NULL) {
        curr_node = curr_node->next_acct;
    }
    if (curr_node == NULL) {
        printf("first node\n");
        curr_node = calloc(1,sizeof(bnk_acct_t));
        *curr_node->name = "first";
    }
    printf("in add acct\b");
    return 0;
}

int load_bnk_accts(bnk_acct_t *list_head) {
    int fd = open("bnk_data", O_CREAT | O_RDWR, 0644);
    if (fd < 0) {
        stan_err("open bnk_data failed");
    }

    bnk_acct_t node_read, *curr_node;
    ssize_t bytes_read;
    size_t struct_size = sizeof(bnk_acct_t);

    while ((bytes_read = read(fd, &node_read, struct_size)) > 0) {
        if (bytes_read < (ssize_t)struct_size) {
            printf("Partial struct read error in load_bnk_accts\n");
            break;
        }
        if (bytes_read < 0) {
            stan_err("Error while reading bnk_accts file");
        }
        if (list_head == NULL) {
            curr_node = calloc(1,sizeof(bnk_acct_t));
            list_head = curr_node;
            curr_node = &node_read;
        } else {
            curr_node = calloc(1,sizeof(bnk_acct_t));
            curr_node->next_acct = &node_read;
            curr_node = curr_node->next_acct;
        }
    }

    return 0;

}