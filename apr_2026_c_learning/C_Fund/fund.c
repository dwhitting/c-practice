#include <stdio.h>
#define TEST_FLOAT (5.0f / 9.0)
#include <ctype.h>
#include <stdbool.h>

int sumArr(int a[], int LEN);
void decompose(double x, long *int_part, double *frac_part);
int *max(int *a, int *b);

int main(void) {

    int i, j;
    i = 6;
    j = 8;
    printf("max: %d\n\n", *max(&i, &j));

    printf("long: %d, int %d\n\n", (int) sizeof(long), (int) sizeof(int));
 


    return 0;
}

int *max(int *a, int *b) {
    if (*a > *b)
        return a;
    else
        return b;
}

void decompose(double x, long *int_part, double *frac_part) {
    *int_part = (int) x;
    *frac_part = x - *int_part;
}

int sumArr(int a[], int LEN) {
    int sum = 0;
    for (int i = 0; i < LEN; i++) {
        sum += a[i];
    }
    return sum;
}