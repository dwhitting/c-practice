#include <stdio.h>
#include "binbin.h"

int main(void)
{
    unsigned input;

    printf("Type a value 0 to 65535: ");
    scanf("%u", &input);
    printf("%u is binary %s\n", input, binbin((unsigned)input));

    return 0;
}