#include <stdio.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
    int i;
    for (i = 0; i < len; i++)
        printf(" %.2x", start[i]);
    printf("\n");
}

void show_int(int x) {
    show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x) {
    show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x) {
    show_bytes((byte_pointer) &x, sizeof(void *));
}

void test_show_bytes(int val) {
    int ival = val;
    int *pval = &ival;
    float fval = (float) ival;
    show_int(ival);
    show_float(fval);
    show_pointer(pval);
    //show_int(*pval);

    printf("\nsize of void*: %lu\n", sizeof(void *));
    
}

int main(void) {
    test_show_bytes(12345);

    return 0;
}