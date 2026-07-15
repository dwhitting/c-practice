#include "stan_hdr.h"

int main() {

    while (1) {
        
        printf("\nMenu: (***Run on Parallels [folder path]***)\n");
        printf("(b) Bank accts\n");
        printf("(l) Bills\n");
        printf("(d) Display\n");
        printf("(q) Quit\n");
        printf("\nEnter selection: ");

        // float t = 1.32;
        // char test[STR_NUM_LEN];
        // float_to_currency(t, test);
        // printf("test: %s\n", test);
        // fflush(stdout);
        
        char ch = single_char_input();

        if (ch == 'q') {
            printf("\n");
            break;
        } else if (ch == 'b') {
            accts_main();
        } else if (ch == 'l') {
            bills_menu();
        } else if (ch == 'd') {
            display_main();
        }
    }
    
    accts_exit();

    return 0;
}