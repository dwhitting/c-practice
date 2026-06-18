#include "my_timer.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

/* This version uses a pipe instead of a temp file to see how 
 * much this could speed things up. */

  typedef struct {
    long elapse_sec;
    float elapse_milli;
} ret_data_t;

ret_data_t single_run(void);

int main(void) 
{
    ret_data_t final_data;
    float sum = 0;
    int runs = 5;

    for (int i = 0; i < runs; i++) {
        final_data = single_run();
        if (final_data.elapse_sec > 0) {
            printf("***One greater than 1 sec...\n");
            break;
        }
        sum += final_data.elapse_milli;
    }

    
    printf("final m: %.2f\n", (sum / runs));

    return 0;
}

ret_data_t single_run(void)
{
    int rows = 10000;
    int cols = 10000;
    int my_pipe[2];
    long received_val = 0;
    long proc_sum = 0;

    my_timer_t timer;

    /* create pipe
     * [1] writes data, [0] receives data */
    if (pipe(my_pipe) == -1) {
        printf("Pipe create failed\n");
        exit(1);
    }

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

    /* fill the array with those 'random' numbers */
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            arr[row][col] = (rand() % 10) +1;
        }
    }

    pid_t child_pid, parent_pid;

    begin_timer(&timer);
    switch (child_pid = fork()) {
        
        case 0:
            close(my_pipe[0]); /* because parent will be receiving */
            for (int row = 0; row < (rows / 2); row++) {
                for (int col = 0; col < cols; col++) {
                    proc_sum += arr[row][col];
                }
            }
            write(my_pipe[1], &proc_sum, sizeof(proc_sum));
            close(my_pipe[1]);
            exit(0);
        default:  /* parent */
            close(my_pipe[1]);
            parent_pid = getpid();
            for (int row = (rows / 2); row < rows; row++) {
                for (int col = 0; col < cols; col++) {
                    proc_sum += arr[row][col];
                }
            }
            read(my_pipe[0], &received_val, sizeof(received_val));
            close(my_pipe[0]);
            proc_sum += received_val; /* final sum of both chunks of the array */
            wait(NULL);            
    }
    end_timer(&timer);

    printf("child pid: %d\n", child_pid);
    printf("parent pid: %d\n", parent_pid);

    printf("Final sum: %ld\n", proc_sum);

    /* free malloc'd stuff */
    for (int row = 0; row < rows; row++)
        free(arr[row]);
    free(arr);

    ret_data_t the_data;
    the_data.elapse_sec = timer.sec_data;
    the_data.elapse_milli = timer.milli_data;
    
    return the_data;    
}