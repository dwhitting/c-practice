#include "stan_hdr.h"

int main() {
    bnk_acct_t *bnk_list = NULL; // = malloc(sizeof(bnk_acct_t));
    load_bnk_accts(bnk_list);

    while (1) {
        
        printf("\nMenu:\n");
        printf("(b) bnk accts\n");
        printf("(q) quit\n");
        char ch = single_char_input();

        if (ch == 'q') {
            break;
        } else if (ch == 'b') {
            bnk_acct_main(bnk_list);
        }
    }
    
   

    return 0;
}