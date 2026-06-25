#include "stan_hdr.h"

int main() {

    while (1) {
        
        printf("\nMenu:\n");
        printf("(b) bnk accts\n");
        printf("(q) quit\n");
        char ch = single_char_input();

        if (ch == 'q') {
            break;
        } else if (ch == 'b') {
            bnk_acct_main();
        }
    }
    
   

    return 0;
}