#ifndef STAN_HDR_H
#define STAN_HDR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#define ACCT_NAME_LEN 100

typedef struct _bnk_acct_t {
    char name[ACCT_NAME_LEN];
    float balance;
    struct _bnk_acct_t *next_acct;
} bnk_acct_t;

int stan_err(char * in_str);
char single_char_input(void);

int bnk_acct_main(void);
int load_bnk_accts(void);

#endif