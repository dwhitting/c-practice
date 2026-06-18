/* This is the first program in my experiment to compare the time it takes for processes 
 * to sum a large dynamic memory array. This file is the single process one */

#include "my_timer.h"

typedef struct {
    long elapse_sec;
    float elapse_milli;
} ret_data_t;

ret_data_t arr_comp();

int main(void)
{
    /* Since I want to compare the same exact arrays, I left this randomizer
     * commented out
     */
    //srand((unsigned int)time(NULL));

    int runs = 10;

    ret_data_t the_data;
    float sum = 0;

    for (int i =0; i < runs; i++) {
        the_data = arr_comp();
        if (the_data.elapse_sec > 0) {
            printf("***One great than 1 sec...\n");
            break;
        }
        sum += the_data.elapse_milli;
    }
        
    printf("final m: %.2f\n", (sum / runs));

    return 0;
}

ret_data_t arr_comp(void) {
    int rows = 10000;
    int cols = 10000;
    my_timer_t timer;

    /* make the array */
    int **arr;
    arr = (int **) malloc(rows * sizeof(int *));
    if (arr == NULL) {
        printf("memory allocation failed\n");
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        arr[i] = (int *) malloc(cols * sizeof(int));
        if (arr[i] == NULL) {
            printf("memory allocation failed\n");
            exit(1);            
        }
    }

    /* fill array with 'random' numbers. Random is in quotes since the seed will always be
     * the same so my speed test will compare apples to apples. */
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            arr[row][col] = (rand() % 10) +1;
        }
    }

    long sum = 0;

    /* actual timed section, which is only for the summing. */
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

    ret_data_t ret_data;
    ret_data.elapse_sec = timer.sec_data;
    ret_data.elapse_milli = timer.milli_data;

    return ret_data;
}