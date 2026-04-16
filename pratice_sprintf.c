#include <stdio.h>
#include <string.h>

int main(void)
{
    char result[20];
    int inputNum = 20;

    sprintf(result, "%5d", inputNum);
    strcat(result, " abc");

    printf("result: %s\n", result);

    return 0;
}