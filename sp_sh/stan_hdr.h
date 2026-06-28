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

enum AcctType {
    bnkAcct,
    credAcct
};

typedef struct {
    enum AcctType acct_Type;
} acct_type_t;

typedef struct _acct_t {
    char name[ACCT_NAME_LEN];
    float balance;
    float cred_lim;
    struct _acct_t *next_acct;
} acct_t;

int stan_err(char * in_str);
char single_char_input(void);

int accts_main(void);
int accts_exit(void);
void read_raw_line(char *buffer, size_t max_len);

#endif