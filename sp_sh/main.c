#include "stan_hdr.h"

int main() {

    while (1) {
        
        printf("\n\nMenu:\n");
        printf("(b) Bnk accts\n");
        printf("(q) Quit\n");
        printf("\nEnter selection: ");
        char ch = single_char_input();

        if (ch == 'q') {
            printf("\n");
            break;
        } else if (ch == 'b') {
            accts_main();
        }
    }
    
    accts_exit();

    return 0;
}