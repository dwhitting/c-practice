#ifndef STAN_HDR_H
#define STAN_HDR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>
#include <math.h>

#define ACCT_NAME_LEN 100
#define STR_NUM_LEN 20
#define DOC_PATH "/media/psf/Documents"

typedef enum {
    bnkAcct,
    credAcct,
    billAcct,
    incomeAcct
} AcctType;

typedef enum {
    None, JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC
} Month;

typedef struct {
    AcctType acct_Type;
} acct_type_t;

typedef struct _acct_t {
    char name[ACCT_NAME_LEN];
    int day;
    Month month;
    int year;
    float balance;
    float cred_lim;
    float cred_remain;
    int date_sort;
    struct _acct_t *next_acct;
} acct_t;

int stan_err(char * in_str);
char single_char_input(void);

int accts_main(void);
int bills_menu(void);
int accts_exit(void);
void read_raw_line(char *buffer, size_t max_len);
char *month_to_str(Month in_month);
int sort_by_date(acct_type_t acct_type, acct_t *input_head);
int num_ll(acct_type_t acct_type);
int set_acct_head(acct_type_t acct_type, acct_t *input_node);
int move_acct_up_one(acct_type_t acct_type);
int list_accts(acct_type_t acct_type);
acct_t *get_acct_head(acct_type_t acct_type);
long raw_read_long(char *prompt);
int raw_read_string(char *prompt, char *ret_string);
int raw_read_int(char *prompt);
float raw_read_float(char *prompt);
int get_date(acct_t *ret_date);
int load_accts(acct_type_t acct_type);
int display_main(void);
int float_to_currency(float in_num, char *ret_str);
float total_bills(void);
int load_all_accts(void);
int income_menu(void);
int last_day_curr_month(void);

#endif