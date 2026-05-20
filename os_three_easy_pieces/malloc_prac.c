#include <stdlib.h>
#include <stdio.h>
#define N 100

int main(void)
{
    int *int_arr = (int *) malloc(N * sizeof(int));

    for (int i = 0; i < N; i++) {
        int_arr[i] = (rand() % (100 - 0 + 1));
    }
    


    for (int i = 0; i < N; i++) {
        printf("res: %d\n", int_arr[i]);
    }

    free(int_arr);
    return 0;
}