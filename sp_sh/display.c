#include "stan_hdr.h"

static int list_bank_accts(float *bank_total);
static int list_cc_accts(float *cc_used_total);
static int list_income(float *income_total);
static int list_bills(float *EOM_assets_minus_bills, float accts_combined_val);

int display_main(void) {

    float est_ret_pay = 5500.0 + 2000.0; // monthly plus VA w/ 2.1k taken out for M

    char temp_curr_1[STR_NUM_LEN];   /* used to store all floats converted to string currency */
    int month_last_day = last_day_curr_month();

    /* list bank accounts */
    float bank_total = 0.0;
    list_bank_accts(&bank_total);

    /* list credit card accounts */
    float cc_used_total = 0.0;
    list_cc_accts(&cc_used_total);

    float assets_minus_cc = bank_total - cc_used_total;
    float_to_currency(assets_minus_cc, temp_curr_1);
    printf("\nBank minus cred card: %s\n", temp_curr_1);

    /* list income and get income total */
    float income_total = 0.0;
    list_income(&income_total);

    /* RET only block */
    printf("        Retired per m: $%.2f\n", est_ret_pay);        
    /* ======= END ========== */

    acct_type_t bills_acct_type = {.acct_Type = billAcct};
    float bill_total = total_acct_balance(bills_acct_type);
    char s_total[STR_NUM_LEN];
    int days_in_curr_month = month_last_day;
    float income_minus_bills = income_total - bill_total;
    float per_day = income_minus_bills / days_in_curr_month;

    /* RET only block */
    float ret_inc_minus_bills = est_ret_pay - bill_total;           
    float ret_per_day = ret_inc_minus_bills / days_in_curr_month;   
    /* ======== END ============*/

    /* list bills */
    float EOM_assets_minus_bills;
    list_bills(&EOM_assets_minus_bills, assets_minus_cc);

    float_to_currency(bill_total, s_total);
    printf("\nTotal Bills: %s\n", s_total);

    float_to_currency(income_minus_bills, s_total);
    printf("\nIncome - Bills: %s\n", s_total);

    /* RET only block */
    float_to_currency(ret_inc_minus_bills, s_total);             
    printf("RET Income - Bills: %s\n", s_total);                    
    /* ============= END ========== */

    float_to_currency(per_day, s_total);
    printf("Per Day: %s\n", s_total);

    /* RET only block */
    float_to_currency(ret_per_day, s_total);                      
    printf("RET Per Day: %s\n", s_total);                       
    /* ======== END =========== */

    acct_t *today = malloc(sizeof(acct_t));
    get_date(today);
    int days_til_month_end = last_day_curr_month() - today->day;
    char s_curr_mon[STR_NUM_LEN];
    strcpy(s_curr_mon, month_to_str(today->month));

    float total_daily_adjust = days_til_month_end * per_day;
    char s_total_daily_adjust[STR_NUM_LEN] = "";
    float_to_currency(total_daily_adjust, s_total_daily_adjust);

    /* RET only block */
    float ret_total_daily_adjust = days_til_month_end * ret_per_day;  
    char s_ret_total_daily_adjust[STR_NUM_LEN] = "";               
    float_to_currency(ret_total_daily_adjust, s_ret_total_daily_adjust);
    /* ======= END ======== */

    float_to_currency(EOM_assets_minus_bills, s_total);
    printf("\nAssets-Bills at EOM: %s (-%s)(%d(s)) (Date: %d %s)\n", 
        s_total, s_total_daily_adjust, days_til_month_end, 
        month_last_day, s_curr_mon);
    float EOM_assets_minus_bills_daily_adjust = EOM_assets_minus_bills - total_daily_adjust;
    float_to_currency(EOM_assets_minus_bills_daily_adjust, s_total);
    printf("%d %s %d Final End of Month: %s\n",today->day, s_curr_mon, 
        today->year, s_total);

    /* for RET only */
    float_to_currency(income_minus_bills, s_total);
    printf("\nRET Pre-end of month: %s (-%s)(%d(s)) (Date: %d %s)\n", 
        s_total, s_ret_total_daily_adjust, days_til_month_end, 
        month_last_day, s_curr_mon);
    float ret_est_end_of_month = income_minus_bills - ret_total_daily_adjust;
    float_to_currency(ret_est_end_of_month, s_total);
    printf("%d %s %d RET Final End of Month: %s\n",today->day, s_curr_mon, 
        today->year, s_total);
    /* ======= END ============== */

    printf("\nAdd Record? (y)...");
    fflush(stdout);
    char ch = single_char_input();
    if (ch == 'y') {
        add_record(EOM_assets_minus_bills_daily_adjust, per_day, days_til_month_end, income_minus_bills);
    }

    free(today);

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

static int list_bills(float *EOM_assets_minus_bills, float accts_combined_val) {

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

    printf("\n      Bills:\n");

    while (curr != NULL) {

        accts_combined_val -= curr->balance;
        if (curr->date_sort >= today->date_sort) {
        }

        float_to_currency(curr->balance, temp_curr_1);
        float_to_currency(accts_combined_val, temp_curr_2);

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
                accts_combined_val += income->balance;
                float_to_currency(income->balance, temp_curr_1);
                float_to_currency(accts_combined_val, temp_curr_2);
                printf("%2d %s %4d %-30s%10s, Actual: %s\n",income->day, month_to_str(income->month), 
                    income->year, income->name, temp_curr_1, temp_curr_2);
            }
            income = income->next_acct;            
        }
        
        if ((curr->next_acct != NULL) && (curr->month < curr->next_acct->month)) {
            *EOM_assets_minus_bills = accts_combined_val;
            printf("*** End of Month ***\n");
        }

        prev = curr;
        curr = curr->next_acct;
    }

    free(today);

    return 0;
}