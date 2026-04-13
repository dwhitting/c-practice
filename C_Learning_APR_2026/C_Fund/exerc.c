#include <stdio.h>

void avg_sum(double a[], int n, double *avg, double *sum);

int main(void) {

    double a[] = {1, 2, 3, 4, 5};
    int n = 5;
    double avg = 0;
    double sum = 0;
    avg_sum(a, n, &avg, &sum);
    printf("avg: %.2f, sum: %.2f\n\n", avg, sum);

    return 0;
}

void avg_sum(double a[], int n, double *avg, double *sum) {
    int i;
    *sum = 0.0;
    for (i = 0; i < n; i++) 
        *sum += a[i];
    *avg = *sum / n;
}