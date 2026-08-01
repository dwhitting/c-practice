#include "stan_hdr.h"

static csv_line_t *csv_linked_list = NULL;

int csv_main(void) {

    int fd = open("budj_est.csv", O_RDONLY);

    if (fd < 0) {
        printf("error opening csv\n");
        exit(0);
    }

    char csv_line_read[CSV_READ_LEN];

    char char_read = '\0';
    bool in_quotes = FALSE;

    int line_cnt = 0;
    int char_cnt = 0;
    csv_line_t *csv_ll_head = NULL;
    csv_line_t *csv_ll_tail = NULL;

    while (read(fd, &char_read, sizeof(char)) > 0) {

        if (char_read == '"') {
            in_quotes = !in_quotes; // toggle quote state
            continue;
        }

        if (char_read == '\n' && !in_quotes) {
            csv_line_read[char_cnt] = '\0';

            csv_line_t *new_line = calloc(1, sizeof(csv_line_t));
            strcpy(new_line->csv_entry, csv_line_read);

            if (csv_ll_head == NULL) {
                csv_ll_head = new_line;
                csv_ll_tail = new_line;
            } else {
                csv_ll_tail->next_rec = new_line;
                csv_ll_tail = csv_ll_tail->next_rec;
            }

            line_cnt++;
            char_cnt = 0;
            continue;
        }

        if (char_cnt < CSV_READ_LEN - 1) {
            csv_line_read[char_cnt] = char_read;
            char_cnt++;
        } else {
            csv_line_read[char_cnt] = '\0';
            printf("\nThere was a buffer overflow\n");
            close(fd);
            exit(0);
        }
    }

    if (char_cnt > 0) {
        csv_line_read[char_cnt] = '\0';

        csv_line_t *new_line = calloc(1, sizeof(csv_line_t));
        strcpy(new_line->csv_entry, csv_line_read);

        if (csv_ll_head == NULL) {
            csv_ll_head = new_line;
            csv_ll_tail = new_line;
        } else {
            csv_ll_tail->next_rec = new_line;
            csv_ll_tail = csv_ll_tail->next_rec;
        }

        line_cnt++;
    }

    close(fd);
    csv_linked_list = csv_ll_head;

    csv_line_t *curr = csv_ll_head;
    int print_cnt = 0;
    while(curr != NULL && print_cnt < 10) {
        printf("%s\n", curr->csv_entry);
        curr = curr->next_rec;
        print_cnt++;
    }
    
    curr = csv_ll_head;
    csv_line_t *prev_curr = NULL;
    while (curr != NULL) {
        prev_curr = curr;
        curr = curr->next_rec;
        free(prev_curr);
    }

    return 0;
}