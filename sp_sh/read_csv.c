#include "stan_hdr.h"

static csv_str_line_t *csv_string_linked_list = NULL;
static int csv_to_text_lines(int *line_count);
static int free_csv_str_ll(void);
static date_t process_date_str(char date_str[20]);
static int parse_csv_line(char line_str[CSV_READ_LEN]);
static int comma_posits(int *in_arr, char line_str[CSV_READ_LEN]);

int csv_main(void) {

    int line_count = 0;

    csv_to_text_lines(&line_count);
    csv_str_line_t *curr = csv_string_linked_list;

    int while_cntr = 0;
    while ((curr != NULL) && (while_cntr < 10)) {
        parse_csv_line(curr->csv_entry);
        curr = curr->next_rec;
        while_cntr++;
    }
    
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

static int parse_csv_line(char line_str[CSV_READ_LEN]) {

    /* skip if first 2x chars are not numbers */
    char test_num_s[3];
    char *endptr;
    memcpy(test_num_s, line_str, 2);
    test_num_s[3] = '\0';
    strtol(test_num_s, &endptr, 10);

    if (test_num_s == endptr) {
        printf("\nSkipping line that doesn't start with 2x num\n");
        return -1;
    }

    int comma_array[20];
    memset(comma_array, 0, sizeof(comma_array));
    comma_posits(comma_array, line_str);

    for (int i = 0; i < 20; i++) {
        printf("%d, ", comma_array[i]);
    }

    return 0;

    /* parse string for date */
    char ch = '\0';
    int char_cnt = 0;
    while (ch != ',') {
        ch = line_str[char_cnt++];
    }

    char date_str[20] = "";
    memcpy(date_str, line_str, 10);

    date_t get_date = process_date_str(date_str);


    if (get_date.day != 0) {
        printf("\nDay: %ld, Mon: %s, Year: %ld\n", get_date.day, 
        month_to_str(get_date.month), get_date.year);
    } else {
        printf("\nLine skipped\n");
    }

    printf("%d\n", char_cnt);

    return 0;
}

static int comma_posits(int *in_arr, char line_str[CSV_READ_LEN]) {

    char ch = '\0';
    int char_cnt = 0;
    int comma_count = 0;

    while (char_cnt < CSV_READ_LEN && ch != '\r') {
        ch = '\0';

        printf("%s\n", line_str);
        while (ch != ',') {
            ch = line_str[char_cnt++];
        }
        in_arr[comma_count++] = char_cnt++;

        if (ch = '\r')
            printf("hit r\n");

    }
    in_arr[comma_count] = char_cnt;

    return 0;
}

static date_t process_date_str(char date_str[20]) {
    date_t ret_date;
    errno = 0;

    char *endptr;

    char month_s[3] = "";
    memcpy(month_s, date_str, 2);
    month_s[3] = '\0';
    Month mon = strtol(month_s, &endptr, 10);

    char day_s[3] = "";
    memcpy(day_s, date_str + 3, 2);
    day_s[3] = '\0';
    long day = strtol(day_s, &endptr, 10);

    if (day_s == endptr) {
        printf("\nError reading date in process_date_Str\n");
        exit(0); 
    }
    

    char year_s[5] = "";
    memcpy(year_s, date_str + 6, 4);
    year_s[5] = '\0';
    int year = atoi(year_s);

    if (day_s == endptr) {
        printf("\nError reading date in process_date_Str\n");
        exit(0); 
    }

    ret_date.day = day;
    ret_date.month = mon;
    ret_date.year = year;

    return ret_date;
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