#include <stdio.h>

char *binbin(unsigned long n) {
    static char bin[17];
    int x;

    for (x = 0; x < 16; x++) {
        bin[x] = n & 0x8000 ? '1' : '0';
        n <<= 1;
    }
    bin[x] = '\0';
    return(bin);

}

int convert_dec_bin(void) {

    unsigned long input;
    char *ret_val;

    printf("Type a value 0 to 65535: ");
    scanf("%lu", &input);
    ret_val = binbin((unsigned long)input);
    printf("%lu is binary %s\n", input, ret_val);

    return 0;
}

int bor(void) {

    const long set = 32;
    unsigned long bor, result;
    printf("Type a value 0 to 65535: ");
    scanf("%lu", &bor);
    result = set | bor;

    printf("\t%s\t%lu\n", binbin((unsigned char)bor), bor);
    printf("|\t%s\t%lu\n", binbin((unsigned char)set), set);
    printf("=\t%s\t%lu\n", binbin((unsigned char)result), result);

    return 0;
}