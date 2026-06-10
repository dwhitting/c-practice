/* This is to compare the time it takes for two processes to sum a large dynamic
 * memory array. This file is the single process. */
#include "my_timer.h"

int arr_comp();

int main(void)
{
    /* Since I want to compare the same exact arrays, I left this system randomizer
     * commented out*/
    //srand((unsigned int)time(NULL));

    arr_comp();

    return 0;
}

int arr_comp(void) {
    int rows = 10000;
    int cols = 10000;
    my_timer_t timer;

    /* make the array */
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

    /* fill array with 'random' numbers. Random is in quotes since the seed will always be
     * the same so my speed test will compare aples to apples. */
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            arr[row][col] = (rand() % 10) +1;
        }
    }

    long sum = 0;

    /* actual times section, which is for only the summing. */
    begin_timer(&timer);
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            sum += arr[row][col];
        }
    }
    end_timer(&timer); 

    printf("sum: %ld\n", sum);

    /* free dynamic mem */
    for (int row = 0; row < rows; row++) {
        free(arr[row]);       
    }
    free(arr);

    return 0;
}