#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int *int_arr = (int *) malloc(100 * sizeof(int));

    //int_arr[100] = 0;

    free(int_arr);

    printf("output: %d\n", int_arr[1]);

    return 0;
}