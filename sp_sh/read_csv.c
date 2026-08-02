#include "stan_hdr.h"

static csv_str_line_t *csv_string_linked_list = NULL;
static int csv_to_text_lines(int *line_count);
static int free_csv_str_ll(void);
static date_t process_date_str(char date_str[20]);
static int parse_csv_line(char line_str[CSV_READ_LEN]);

int csv_main(void) {

    int line_count = 0;

    csv_to_text_lines(&line_count);
    csv_str_line_t *curr = csv_string_linked_list;
    curr = curr->next_rec; //skip header
    parse_csv_line(curr->csv_entry);

    curr = csv_string_linked_list;
    int print_cnt = 0;
    while(curr != NULL && print_cnt < 3) {
        printf("%s\n", curr->csv_entry);
        curr = curr->next_rec;
        print_cnt++;
    }
    
    free_csv_str_ll();

    printf("\nlines: %d\n", line_count);

    return 0;
}

static date_t process_date_str(char date_str[20]) {
    date_t ret_date;

    char month_s[3] = "";
    memcpy(month_s, date_str, 2);
    month_s[3] = '\0';
    Month mon = atoi(month_s);

    char day_s[3] = "";
    memcpy(day_s, date_str + 3, 2);
    day_s[3] = '\0';
    int day = atoi(day_s);
    

    char year_s[5] = "";
    memcpy(year_s, date_str + 6, 4);
    year_s[5] = '\0';
    int year = atoi(year_s);

    ret_date.day = day;
    ret_date.month = mon;
    ret_date.year = year;

    return ret_date;
}

static int parse_csv_line(char line_str[CSV_READ_LEN]) {
    char ch = '\0';
    int char_cnt = 0;
    while (ch != ',') {
        ch = line_str[char_cnt++];
    }

    char date_str[20] = "";
    memcpy(date_str, line_str, 10);

    date_t get_date = process_date_str(date_str);

    printf("\nDay: %d, Mon: %s, Year: %d\n", get_date.day, 
        month_to_str(get_date.month), get_date.year);


    printf("%s\n", date_str);


    printf("%d\n", char_cnt);

    return 0;
}

static int free_csv_str_ll(void) {
    csv_str_line_t *curr = csv_string_linked_list;
    csv_str_line_t *prev_curr = NULL;
    while (curr != NULL) {
        prev_curr = curr;
        curr = curr->next_rec;
        free(prev_curr);
    }

    return 0;
}

static int csv_to_text_lines(int *line_count) {

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
    csv_str_line_t *csv_ll_head = NULL;
    csv_str_line_t *csv_ll_tail = NULL;

    while (read(fd, &char_read, sizeof(char)) > 0) {

        if (char_read == '"') {
            in_quotes = !in_quotes; // toggle quote state
            continue;
        }

        if (char_read == '\n' && !in_quotes) {
            csv_line_read[char_cnt] = '\0';

            csv_str_line_t *new_line = calloc(1, sizeof(csv_str_line_t));
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

        csv_str_line_t *new_line = calloc(1, sizeof(csv_str_line_t));
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
    csv_string_linked_list = csv_ll_head;
    *line_count = line_cnt;

    return 0;
}