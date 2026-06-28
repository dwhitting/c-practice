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

#define ACCT_NAME_LEN 100

typedef enum {
    bnkAcct,
    credAcct
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
    float balance;
    float cred_lim;
    float cred_remain;
    int date_sort;
    struct _acct_t *next_acct;
} acct_t;

int stan_err(char * in_str);
char single_char_input(void);

int accts_main(void);
int accts_exit(void);
void read_raw_line(char *buffer, size_t max_len);
char *month_to_str(Month in_month);
int sort_by_date(acct_t *input_head);
int num_ll(acct_type_t acct_type);

#endif