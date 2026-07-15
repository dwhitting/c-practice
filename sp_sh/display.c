#include "stan_hdr.h"

int display_main(void) {

    acct_type_t acct_type;
    for (acct_type.acct_Type = 0; acct_type.acct_Type < 3;  acct_type.acct_Type++) {
        load_accts(acct_type);
    }

    char temp_curr_1[STR_NUM_LEN], 
         temp_curr_2[STR_NUM_LEN];  /* used to store all floats converted to string currency */

    /* list bank accounts */
    acct_t *curr;
    acct_type.acct_Type = bnkAcct;
    curr = get_acct_head(acct_type);

    float bank_total = 0.0;
    printf("\nBank Accounts:\n");
    while (curr != NULL) {
        float_to_currency(curr->balance, temp_curr_1);
        printf("%-14s %15s\n", curr->name, temp_curr_1);
        bank_total += curr->balance;
        curr = curr->next_acct;
    }
    float_to_currency(bank_total, temp_curr_1);
    printf("          Total: %13s\n", temp_curr_1);

    /* list credit card accounts */
    acct_type.acct_Type = credAcct;
    curr = get_acct_head(acct_type);

    float cc_used = 0.0;
    float cc_used_total = 0.0;
    printf("\nCredit Cards:\n");
    printf("Name\t\t   Cred Used\n");
    while (curr != NULL) {
        cc_used = curr->cred_lim - curr->cred_remain;
        float_to_currency(cc_used, temp_curr_1);
        printf("%-14s%15s\n", curr->name, temp_curr_1);
        cc_used_total += cc_used;
        curr = curr->next_acct;
    }
    float_to_currency(cc_used_total, temp_curr_1);
    printf("         Total: %13s\n", temp_curr_1);

    float accts_combined_val = bank_total - cc_used_total;
    float_to_currency(accts_combined_val, temp_curr_1);
    printf("\n Combined Accts Val: $%s\n", temp_curr_1);

    /* list bills */
    acct_type.acct_Type = billAcct;
    acct_t *today = malloc(sizeof(acct_t));
    get_date(today);
    sort_by_date(get_acct_head(acct_type));
    curr = get_acct_head(acct_type);
    printf("\nBills:\n");
    while (curr != NULL) {
        accts_combined_val -= curr->balance;
        float_to_currency(curr->balance, temp_curr_1);
        float_to_currency(accts_combined_val, temp_curr_2);
        printf("%2d %s %4d %-25s%15s, New Total: %s\n",curr->day, month_to_str(curr->month), 
            curr->year, curr->name, temp_curr_1, temp_curr_2);
        if (curr->next_acct == NULL && curr->date_sort <= today->date_sort) {
            printf("TODAY: %2d %s %4d\n", today->day, month_to_str(today->month), today->year);
        } else if ((curr->date_sort <= today->date_sort) && (curr->next_acct->date_sort > today->date_sort)) {
            printf("TODAY: %2d %s %4d\n", today->day, month_to_str(today->month), today->year);
        }
        curr = curr->next_acct;
    }



    printf("\nPress key to continue...");
    fflush(stdout);
    single_char_input();

    free(today);

    return 0;
}