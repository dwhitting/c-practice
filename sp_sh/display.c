#include "stan_hdr.h"

int display_main(void) {

    acct_type_t acct_type;
    for (acct_type.acct_Type = 0; acct_type.acct_Type < 3;  acct_type.acct_Type++) {
        load_accts(acct_type);
    }

    /* list bank accounts */
    acct_t *curr;
    acct_type.acct_Type = bnkAcct;
    curr = get_acct_head(acct_type);

    float bank_total = 0.0;
    printf("\nBank Accounts:\n");
    while (curr != NULL) {
        printf("%-14s %15.2f\n", curr->name, curr->balance);
        bank_total += curr->balance;
        curr = curr->next_acct;
    }
    printf("          Total: %13.2f\n", bank_total);

    /* list credit card accounts */
    acct_type.acct_Type = credAcct;
    curr = get_acct_head(acct_type);

    float cc_used = 0.0;
    float cc_used_total = 0.0;
    printf("\nCredit Cards:\n");
    printf("Name\t\t   Cred Used\n");
    while (curr != NULL) {
        cc_used = curr->cred_lim - curr->cred_remain;
        printf("%-14s%15.2f\n", curr->name, cc_used);
        cc_used_total += cc_used;
        curr = curr->next_acct;
    }
    printf("         Total: %13.2f\n", cc_used_total);

    float accts_combined_val = bank_total - cc_used_total;
    printf("\n Combined Accts Val: $%.2f\n", accts_combined_val);

    /* list bills */
    acct_type.acct_Type = billAcct;
    acct_t *today = malloc(sizeof(acct_t));
    get_date(today);
    sort_by_date(get_acct_head(acct_type));
    curr = get_acct_head(acct_type);
    printf("\nBills:\n");
    while (curr != NULL) {
        accts_combined_val -= curr->balance;
        printf("%2d %s %4d %-14s%15.2f, New Total: $%.2f\n",curr->day, month_to_str(curr->month), 
            curr->year, curr->name, curr->balance, accts_combined_val);
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