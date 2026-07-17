#include "stan_hdr.h"

static int list_bank_accts(float *bank_total);
static int list_cc_accts(float *cc_used_total);
static int list_income(float *income_total);
static int list_bills(float *end_of_month, float accts_combined_val, float per_day);

int display_main(void) {

    char temp_curr_1[STR_NUM_LEN];   /* used to store all floats converted to string currency */
    int month_last_day = last_day_curr_month();

    /* list bank accounts */
    float bank_total = 0.0;
    list_bank_accts(&bank_total);

    /* list credit card accounts */
    float cc_used_total = 0.0;
    list_cc_accts(&cc_used_total);

    float accts_combined_val = bank_total - cc_used_total;
    float_to_currency(accts_combined_val, temp_curr_1);
    printf("\nCombined Accts Val: $%s\n", temp_curr_1);

    /* list income */
    float income_total = 0.0;
    list_income(&income_total);

    float bill_total = total_bills();
    char s_total[STR_NUM_LEN];
    float inc_minus_bills = income_total - bill_total;
    float per_day = inc_minus_bills / 30; /* temp div day (month) */

    /* list bills */
    float end_of_month = 0;
    list_bills(&end_of_month, accts_combined_val, per_day);

    float_to_currency(bill_total, s_total);
    printf("\nTotal Bills: %s\n", s_total);

    float_to_currency(inc_minus_bills, s_total);
    printf("\nIncome - Bills: %s\n", s_total);
    
    float_to_currency(per_day, s_total);
    printf("Per Day: %s\n", s_total);

    float_to_currency(end_of_month, s_total);
    printf("\nEnd of month: %s (Day: %d)\n", s_total, month_last_day);

    printf("\nPress key to continue...");
    fflush(stdout);
    single_char_input();

    return 0;
}

static int list_bank_accts(float *bank_total) {

    char temp_curr_1[STR_NUM_LEN];   /* used to store all floats converted to string currency */
    acct_type_t acct_type;
    acct_t *curr;
    acct_type.acct_Type = bnkAcct;
    curr = get_acct_head(acct_type);

    
    printf("\n      Bank Accounts:\n");
    while (curr != NULL) {
        float_to_currency(curr->balance, temp_curr_1);
        printf("%-14s %15s\n", curr->name, temp_curr_1);
        *bank_total += curr->balance;
        curr = curr->next_acct;
    }
    float_to_currency(*bank_total, temp_curr_1);
    printf("          Total: %13s\n", temp_curr_1);

    return 0;
}

static int list_cc_accts(float *cc_used_total) {

    char temp_curr_1[STR_NUM_LEN];  /* used to store all floats converted to string currency */

    acct_type_t acct_type;
    acct_type.acct_Type = credAcct;
    acct_t *curr;
    curr = get_acct_head(acct_type);

    float cc_used = 0.0;
    printf("\n      Credit Cards:\n");
    printf("Name\t\t   Cred Used\n");
    while (curr != NULL) {
        cc_used = curr->cred_lim - curr->cred_remain;
        float_to_currency(cc_used, temp_curr_1);
        printf("%-14s%15s\n", curr->name, temp_curr_1);
        *cc_used_total += cc_used;
        curr = curr->next_acct;
    }
    float_to_currency(*cc_used_total, temp_curr_1);
    printf("         Total: %13s\n", temp_curr_1);

    return 0;
}

static int list_income(float *income_total) {

    char temp_curr_1[STR_NUM_LEN];  /* used to store all floats converted to string currency */

    acct_type_t acct_type;
    acct_t *curr;
    acct_type.acct_Type = incomeAcct;
    curr = get_acct_head(acct_type);

    
    printf("\n      Income:\n");
    while (curr != NULL) {
        float_to_currency(curr->balance, temp_curr_1);
        printf("%-14s %15s\n", curr->name, temp_curr_1);
        *income_total += curr->balance;
        curr = curr->next_acct;
    }
    float_to_currency(*income_total, temp_curr_1);
    printf("          Total: %13s\n", temp_curr_1);

    return 0;
}

static int list_bills(float *end_of_month, float accts_combined_val, float per_day) {

    char temp_curr_1[STR_NUM_LEN], 
         temp_curr_2[STR_NUM_LEN];  /* used to store all floats converted to string currency */

    acct_type_t acct_income;
    acct_income.acct_Type = incomeAcct;
    acct_t *income; 

    acct_type_t acct_bill;
    acct_t *curr;
    acct_t *prev;
    acct_t *bill_head;
    acct_bill.acct_Type = billAcct;
    acct_t *today = malloc(sizeof(acct_t));
    get_date(today);
    sort_by_date(acct_bill, get_acct_head(acct_bill));
    curr = get_acct_head(acct_bill);
    bill_head = get_acct_head(acct_bill);
    prev = curr;
    char s_per_day[STR_NUM_LEN] = "";
    float_to_currency(per_day, s_per_day);
    int month_last_day = last_day_curr_month();

    printf("\n      Bills:\n");

    while (curr != NULL) {

        accts_combined_val -= curr->balance;
        if ((bill_head == curr && curr->date_sort == today->date_sort) 
            || ((prev->date_sort < curr->date_sort) && (curr->date_sort >= today->date_sort))) {
            accts_combined_val -= per_day;
        }

        float_to_currency(curr->balance, temp_curr_1);
        float_to_currency(accts_combined_val, temp_curr_2);

        if (curr->next_acct == NULL && curr->date_sort <= today->date_sort) {
            printf("TODAY: %2d %s %4d\n", today->day, month_to_str(today->month), today->year);
        } else if ((curr->date_sort <= today->date_sort) && (curr->next_acct->date_sort > today->date_sort)) {
            printf("TODAY: %2d %s %4d\n", today->day, month_to_str(today->month), today->year);
        } else if (curr == bill_head && bill_head->date_sort > today->date_sort) {
            printf("TODAY: %2d %s %4d\n", today->day, month_to_str(today->month), today->year);
        }

        printf("%2d %s %4d %-30s%10s, Actual: %s\n",curr->day, month_to_str(curr->month), 
            curr->year, curr->name, temp_curr_1, temp_curr_2);

        income = get_acct_head(acct_income); 
        while (income != NULL) { 
            if (((curr == bill_head && curr->date_sort == income->date_sort)) || 
                ((prev->date_sort < curr->date_sort) && (curr->date_sort == income->date_sort))) {
                accts_combined_val += income->balance;
                float_to_currency(income->balance, temp_curr_1);
                float_to_currency(accts_combined_val, temp_curr_2);
                printf("%2d %s %4d %-30s%10s, Actual: %s\n",income->day, month_to_str(income->month), 
                    income->year, income->name, temp_curr_1, temp_curr_2);
            }
            income = income->next_acct;
        }

        if ((curr->next_acct != NULL) && (curr->month < curr->next_acct->month)) {
            printf("*** End of Month ***\n");
            *end_of_month = accts_combined_val;
        }

        prev = curr;
        curr = curr->next_acct;
    }

    free(today);

    return 0;
}