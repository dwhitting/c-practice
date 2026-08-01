#include "stan_hdr.h"

static int list_bank_accts(float *bank_total);
static int list_cc_accts(float *cc_used_total);
static int list_income(float *income_total);
static int list_bills(float *EOM_assets_minus_bills, float accts_combined_val);
static int compare_by_date(const void *a, const void *b);

int display_main(void) {

    //float RET_income_total = 5500.0 + 2000.0; // monthly plus VA w/ 2.1k taken out for M

    char s_temp[STR_NUM_LEN];   /* used to store all floats converted to string currency */
    int month_last_day = last_day_curr_month();
    int days_in_curr_month = month_last_day;

    /* list bank accounts */
    float assets_total = 0.0;
    list_bank_accts(&assets_total);

    /* list credit card accounts */
    float cc_used_total = 0.0;
    list_cc_accts(&cc_used_total);

    float_to_currency((assets_total - cc_used_total), s_temp);
    printf("\nAssets - cred used: %s\n", s_temp);

    /* list income and get income total */
    float income_total = 0.0;
    list_income(&income_total);

    acct_type_t bills_acct_type = {.acct_Type = billAcct};
    float bills_total = total_acct_balance(bills_acct_type);
    float per_day = (income_total - bills_total) / days_in_curr_month;

    /* list bills */
    float EOM_assets_minus_bills;
    list_bills(&EOM_assets_minus_bills, (assets_total - cc_used_total));

    float_to_currency(bills_total, s_temp);
    printf("\nTotal Bills: %s\n", s_temp);

    float_to_currency((income_total - bills_total), s_temp);
    printf("\nIncome - Bills: %s\n", s_temp);

    float_to_currency(per_day, s_temp);
    printf("Per Day: %s\n", s_temp);
    float_to_currency(per_day * 7, s_temp);
    printf("Per Week: %s\n", s_temp);
    float day_spend = 70.0;
    float_to_currency((per_day - day_spend) *7, s_temp);
    printf("After $70 spent per day (7d) %s\n", s_temp);
    float_to_currency((per_day - day_spend) * 30, s_temp);
    printf("After $70 spent per day (30d): %s\n", s_temp);

    acct_t *today = malloc(sizeof(acct_t));
    get_date(today);
    int days_til_month_end = month_last_day - today->day;
    char s_curr_mon[STR_NUM_LEN];
    strcpy(s_curr_mon, month_to_str(today->month));

    float daily_adjust_to_EOM = days_til_month_end * per_day;
    char s_daily_adjust_to_EOM[STR_NUM_LEN] = "";
    float_to_currency(daily_adjust_to_EOM, s_daily_adjust_to_EOM);

    float_to_currency(EOM_assets_minus_bills, s_temp);
    printf("\nEOM Assets-Bills: %s (-%s)(%d(s)) (Date: %d %s)\n", 
        s_temp, s_daily_adjust_to_EOM, days_til_month_end, 
        month_last_day, s_curr_mon);
    float_to_currency((EOM_assets_minus_bills - daily_adjust_to_EOM), s_temp);
    printf("%d %s %d Final EOM Assets-Bills-Daily Adj: %s\n",today->day, s_curr_mon, 
        today->year, s_temp);

    /* add new record */
    printf("\nAdd Record? (y)...");
    fflush(stdout);
    char ch = single_char_input();
    if (ch == 'y') {

        record_t *new_record = calloc(1, sizeof(record_t));
        if (new_record == NULL) {
            printf("calloc failed in add record\n");
            return 1;
        }

        new_record->day = today->day;
        new_record->month = today->month;
        new_record->year = today->year;
        new_record->date_sort = today->date_sort;
        new_record->days_in_month = days_in_curr_month;
        new_record->days_till_EOM = days_til_month_end;
        new_record->assets_total = assets_total;
        new_record->cc_used_total = cc_used_total;
        new_record->income_total = income_total;
        new_record->per_day = per_day;

        new_record->bills_total = bills_total;
        new_record->EOM_assets_minus_bills = EOM_assets_minus_bills;

        add_record(new_record);
    }

    free(today);

    return 0;
}

static int list_bank_accts(float *assets_total) {

    char temp_curr_1[STR_NUM_LEN];   /* used to store all floats converted to string currency */
    acct_type_t acct_type;
    acct_t *curr;
    acct_type.acct_Type = bnkAcct;
    curr = get_acct_head(acct_type);

    
    printf("\n      Assets:\n");
    while (curr != NULL) {
        float_to_currency(curr->balance, temp_curr_1);
        printf("%-14s %15s\n", curr->name, temp_curr_1);
        *assets_total+= curr->balance;
        curr = curr->next_acct;
    }
    float_to_currency(*assets_total, temp_curr_1);
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

static int list_bills(float *EOM_assets_minus_bills, float curr_assets_minus_cc_used) {

    char temp_curr_1[STR_NUM_LEN], 
         temp_curr_2[STR_NUM_LEN];  /* used to store all floats converted to string currency */

    if (ws == AD) {
        printf("\n      Bills:\n");
    } else if (ws == RET) {
        printf("\n (RET)  Bills:\n");
    }

    acct_type_t income_acct = {.acct_Type = incomeAcct};
    acct_type_t bill_acct = {.acct_Type = billAcct};
    acct_t *income_head = get_acct_head(income_acct); 
    acct_t *bill_head = get_acct_head(bill_acct);

    acct_t *today = calloc(1, sizeof(acct_t));
    get_date(today);

    int MAX_COMBINED = 256;
    typedef enum { no, yes } EOM_printed_E;
    EOM_printed_E EOM_printed = no;

    acct_t *comb_arr[MAX_COMBINED];
    int comb_cnt = 0;

    acct_t *curr_bill = bill_head;

    while (curr_bill != NULL && comb_cnt < MAX_COMBINED) {
        comb_arr[comb_cnt++] = curr_bill;
        curr_bill = curr_bill->next_acct;
    }

    acct_t *curr_income = income_head;

    while (curr_income != NULL && comb_cnt < MAX_COMBINED) {
        comb_arr[comb_cnt++] = curr_income;
        curr_income = curr_income->next_acct;
    }

    qsort(comb_arr, comb_cnt, sizeof(acct_t *), compare_by_date);

    for (int i = 0; i < comb_cnt; i++) {
        if (comb_arr[i]->acctType == incomeAcct) {
            curr_assets_minus_cc_used += comb_arr[i]->balance;
        } else {
            curr_assets_minus_cc_used -= comb_arr[i]->balance;
        }
        

        if (i == 0 && comb_arr[i]->date_sort > today->date_sort) {
            printf("*** TODAY: %2d %s %4d ***\n", today->day, month_to_str(today->month), today->year);
        }

        float_to_currency(comb_arr[i]->balance, temp_curr_1);
        float_to_currency(curr_assets_minus_cc_used, temp_curr_2);
        printf("%2d %s %4d %-30s%10s, Actual: %s\n",comb_arr[i]->day, month_to_str(comb_arr[i]->month), 
            comb_arr[i]->year, comb_arr[i]->name, temp_curr_1, temp_curr_2);

        if ((i == comb_cnt - 1) && comb_arr[i]->date_sort <= today->date_sort) {
            printf("*** TODAY: %2d %s %4d ***\n", today->day, month_to_str(today->month), today->year);
        } else if ((comb_arr[i]->date_sort <= today->date_sort) && (comb_arr[i + 1]->date_sort > today->date_sort)) {
            printf("*** TODAY: %2d %s %4d ***\n", today->day, month_to_str(today->month), today->year);
        }

        if ((today->month == comb_arr[i]->month) && (i < comb_cnt - 1) &&
            (comb_arr[i + 1]->month > today->month)) {
            *EOM_assets_minus_bills = curr_assets_minus_cc_used;
            printf("*** End of Month ***\n");
            EOM_printed = yes;
        }

        if ((i == comb_cnt -1) && (EOM_printed == no)) {
            *EOM_assets_minus_bills = curr_assets_minus_cc_used;
            printf("*** End of Month ***\n");
        }

    }

    free(today);

    return 0;
}

static int compare_by_date(const void *a, const void *b) {
    acct_t *acct_a = *(acct_t **)a;
    acct_t *acct_b = *(acct_t **)b;
    return acct_a->date_sort - acct_b->date_sort;
}

