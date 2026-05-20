/*
Example of why gets was depricated. if response[] is no large enough,
an overflow happens

*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int get_y_or_no(void) {
    char resoponse[5];
    puts("Continue? [y] n: ");
    gets(resoponse);
    if (tolower(resoponse[0]) == 'n') return 0;
    return -1;
}

int main(void)
{
    int i = get_y_or_no();
    printf("i: %d\n", i);
    return 0;
}