#include "stan_hdr.h"

int main() {

    while (1) {
        
        printf("\nMenu:\n");
        printf("(b) Bnk accts\n");
        printf("(q) Quit\n");
        char ch = single_char_input();

        if (ch == 'q') {
            break;
        } else if (ch == 'b') {
            accts_main();
        }
    }
    
    accts_exit();

    return 0;
}