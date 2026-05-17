#include <stdio.h>
#include "my_str_functions.h"
#define BUFF_SIZE 5

/*
 * This is to practice recreating some basic string functions
*/

int main(void)
{
    char my_str[BUFF_SIZE] = "";
    my_get_str(my_str, BUFF_SIZE);


    printf("the string: %s\n", my_str);
    printf("char count: %d\n", my_len(my_str));

    return 0;
}