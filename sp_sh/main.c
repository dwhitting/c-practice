#include "stan_hdr.h"

work_status ws = AD;

int main() {

    struct stat st;
    if (stat(DOC_PATH, &st) != 0) {
        printf("Does not look like running from Paralles\n");
        return 0;
    }

    load_all_accts();

    while (1) {
        
        printf("\nMenu: (***Run on Parallels [folder path]***)\n");
        printf("(A) AD, (R) RET\n");
        printf("(b) Bank and Credit Accts\n");
        printf("(l) Bills\n");
        printf("(i) Income\n");
        printf("(d) Display\n");
        printf("(r) Records\n");
        printf("(q) Quit\n");
        printf("\nEnter selection: ");
        
        char ch = single_char_input();

        if (ch == 'q') {
            printf("\n");
            break;
        } else if (ch == 'b') {
            accts_main();
        } else if (ch == 'l') {
            bills_menu();
        } else if (ch == 'i') {
            income_menu();
        } else if (ch == 'd') {
            display_main();
        } else if (ch == 'r') {
            records_menu();
        } else if (ch == 'A') {
            printf("\nSet of AD\n\n");
            save_all_accts();
            accts_exit();
            ws = AD;
            load_all_accts();
        } else if (ch == 'R') {
            printf("\nSet to RET\n\n");
            save_all_accts();
            accts_exit();
            ws = RET;
            load_all_accts();
        }
    }
    
    accts_exit();

    return 0;
}