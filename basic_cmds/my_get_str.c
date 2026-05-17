#include <stdio.h>
#include <stdlib.h>
#define INIT_MALLOC 5

int my_len(char *);

char *my_get_str(void) {

    printf("Enter string: ");
    char ch;
    int placer = 0;
    int current_size = INIT_MALLOC;

    char *ret_str = (char *) malloc(INIT_MALLOC * sizeof(char));
    if (ret_str == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    while ((ch = getchar()) != '\n' && ch != EOF) {

        if (placer == current_size -2) {
            current_size = current_size * 2;
            char *temp = realloc(ret_str, current_size * sizeof(char));
            if (temp == NULL) {
                printf("Memory realloc failed.\n");
                free(ret_str);
                return NULL;
            } else {
                printf("New size %d\n", current_size);
            }
            ret_str = temp;
        }

        ret_str[placer++] = ch;
    }
    ret_str[placer] = '\0';

    return ret_str;
}

