#include <stdio.h>
#include <stdlib.h>
#include "my_str_functions.h"

/*
 * This is to practice recreating some basic string functions
*/

int main(void)
{
    char *my_str = my_get_str();


    printf("the string: %s\n", my_str);
    printf("char count: %d\n", my_len(my_str));

    if (my_str != NULL) {
        free(my_str);
        my_str = NULL;
    }

    return 0;
}