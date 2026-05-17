#include <stdio.h>

int my_len(char *);

void my_get_str(char *mod_str, int buff_size) {

    printf("Enter string: ");
    char ch;
    int placer = 0;

    while ((ch = getchar()) != '\n') {
        mod_str[placer++] = ch;
        if (placer == buff_size -1) {
            printf("Buffer exceeded. String cut to %d chars\n", buff_size -1);    
            break;
        }
    }
    mod_str[placer] = '\0';
}

