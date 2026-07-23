#include "stan_hdr.h"

static int list_bank_accts(float *bank_total);
static int list_cc_accts(float *cc_used_total);
static int list_income(float *income_total);
static int list_bills(float *EOM_assets_minus_bills, float accts_combined_val);

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
    list_bills(&EOM_assets_minus_bills, (assets_total- cc_used_total));

    float_to_currency(bills_total, s_temp);
    printf("\nTotal Bills: %s\n", s_temp);

    float_to_currency((income_total - bills_total), s_temp);
    printf("\nIncome - Bills: %s\n", s_temp);

    float_to_currency(per_day, s_temp);
    printf("Per Day: %s\n", s_temp);

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

    if (ws == AD) {
        printf("\n      Bills:\n");
    } else if (ws == RET) {
        printf("\n (RET)  Bills:\n");
    }
    

    while (curr != NULL) {

        curr_assets_minus_cc_used -= curr->balance;
        if (curr->date_sort >= today->date_sort) {
        }

        float_to_currency(curr->balance, temp_curr_1);
        float_to_currency(curr_assets_minus_cc_used, temp_curr_2);

        if ((curr == bill_head) && (bill_head->date_sort > today->date_sort)) {
            printf("*** TODAY: %2d %s %4d ***\n", today->day, month_to_str(today->month), today->year);
        }

        printf("%2d %s %4d %-30s%10s, Actual: %s\n",curr->day, month_to_str(curr->month), 
            curr->year, curr->name, temp_curr_1, temp_curr_2);

        if (curr->next_acct == NULL && curr->date_sort <= today->date_sort) {
            printf("*** TODAY: %2d %s %4d ***\n", today->day, month_to_str(today->month), today->year);
        } else if ((curr->date_sort <= today->date_sort) && (curr->next_acct->date_sort > today->date_sort)) {
            printf("*** TODAY: %2d %s %4d ***\n", today->day, month_to_str(today->month), today->year);
        }
            /* prob unecesary looping that could increase efficency */
        income = get_acct_head(acct_income); 
        while (income != NULL) { 
            if ((((curr == bill_head && curr->date_sort) && curr->date_sort == income->date_sort)) || 
                ((prev->date_sort < curr->date_sort) && (curr->date_sort == income->date_sort))) {
                curr_assets_minus_cc_used += income->balance;
                float_to_currency(income->balance, temp_curr_1);
                float_to_currency(curr_assets_minus_cc_used, temp_curr_2);
                printf("%2d %s %4d %-30s%10s, Actual: %s\n",income->day, month_to_str(income->month), 
                    income->year, income->name, temp_curr_1, temp_curr_2);
            }
            income = income->next_acct;            
        }
        
        if ((curr->next_acct != NULL) && (curr->month < curr->next_acct->month)) {
            *EOM_assets_minus_bills = curr_assets_minus_cc_used;
            printf("*** End of Month ***\n");
        }

        prev = curr;
        curr = curr->next_acct;
    }

    free(today);

    return 0;
}