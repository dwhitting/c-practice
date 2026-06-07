#include "my_timer.h"

int arr_comp();

int main(void)
{
    my_timer_t timer;

    begin_timer(&timer);

    sleep(2);

    end_timer(&timer);

    arr_comp();
    

    return 0;
}

int arr_comp(void) {
    int rows = 100;
    int cols = 100;

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

    

    printf("rand: %d", (rand() % 10) +1);

    return 0;


}