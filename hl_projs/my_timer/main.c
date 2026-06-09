#include "my_timer.h"

int arr_comp();

int main(void)
{
    srand((unsigned int)time(NULL));

    my_timer_t timer;

    begin_timer(&timer);

    //sleep(2);
    arr_comp();

    end_timer(&timer);    

    return 0;
}

int arr_comp(void) {
    int rows = 1000;
    int cols = 1000;

    int **arr;
    arr = (int **) malloc(rows * sizeof(int *));
    if (arr == NULL) {
        printf("memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < rows; i++) {
        arr[i] = (int *) malloc(cols * sizeof(int));
        if (arr[i] == NULL) {
            printf("memory allocation failed\n");
            return 1;            
        }
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            arr[row][col] = (rand() % 10) +1;
        }
    }

    int sum = 0;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            sum += arr[row][col];
        }
    }
    printf("sum: %d\n", sum);

    for (int row = 0; row < rows; row++) {
        free(arr[row]);       
    }
    free(arr);

    return 0;
}