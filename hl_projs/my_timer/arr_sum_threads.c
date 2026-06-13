#include "wrap.h"
#include "my_timer.h"

/* custom thread data structure that holds each threads id and 
 * the sum it computes
 */
typedef struct {
    int thrd_id;
    int ret_sum;
} thread_data_t;

/* moved arr to global so all threads can access it */
int **arr;

/* thread figures out which 1000 rows it is suming, sums them, 
 * and puts the resut in the custom thread_data_t structure 
 */
void *thread(void *vargp) {
    thread_data_t *data = (thread_data_t *)vargp;
    int begin = data->thrd_id * 1000;
    int end = begin + 1000;
    int t_ret_sum = 0;

    printf("beg: %d, end %d\n", begin, end);
    for (int i = begin; i < end; i++) {
        for (int j = 0; j < 10000; j++)
            t_ret_sum += arr[i][j];
    }
    data->ret_sum = t_ret_sum;
    
    return NULL;
}

int main(void) 
{

    pthread_t tid_holder[10];
    thread_data_t thread_args[10];

    /* Since I want to compare the same exact arrays, I left this randomizer
     * commented out
     */
    //srand((unsigned int)time(NULL));


    my_timer_t timer;

    /* make the array */
    int rows = 10000;
    int cols = 10000;
    
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
     * the same so my speed test will compare apples to apples. 
     */
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            arr[row][col] = (rand() % 10) +1;
        }
    }

    begin_timer(&timer);

    for (int i = 0; i < 10; i++) {
        thread_args[i].thrd_id = i;
        Pthread_create(&tid_holder[i], NULL, thread, &thread_args[i]);
    }

    long final_sum = 0;
    for (int i = 0; i < 10; i++) {
        pthread_join(tid_holder[i], NULL);
        final_sum += thread_args[i].ret_sum;
    }
    end_timer(&timer);

    printf("final sum: %ld\n", final_sum);

    

    /* free dynamic mem */
    for (int row = 0; row < rows; row++) {
        free(arr[row]);       
    }
    free(arr);

    return 0;
}

