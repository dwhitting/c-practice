#include "my_timer.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <string.h>

/* 
 * This is to time summing the array in two chunks: the first half in a child
 * process and the second half in the parent process. 
 */

 typedef struct {
    long elapse_sec;
    float elapse_milli;
} ret_data_t;

ret_data_t single_run(void);

int main(void) 
{
    ret_data_t final_data;
    float sum = 0;
    int runs = 10;

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

    /* Just for my file handling learning, I have each process right their chunk of the 
     * array sum to a temp file, then read and sum them at the end for the final total. */
    char template[] = "/tmp/tempFileXXXXXX";
    long cp_input = 0;
    long read_string = 0;
    long proc_sum = 0;
    my_timer_t timer;
    
    int fd = mkstemp(template);
    printf("file name: %s\n", template);

    /* make the array in the same way as I did in the single process file. */
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

    /* fill the same way as in the single process file. */
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            arr[row][col] = (rand() % 10) +1;
        }
    }

    pid_t child_pid, parent_pid;

    /* 
     * Begin the timer, fork into parent and child, and have each process (parent and child)
     * sum their portion and save it to the tempfile 
     * Even with the file writes within the timed block, it is still faster than the single
     * process version.  Wow!
     */
    begin_timer(&timer);
    switch (child_pid = fork()) {
        
        case 0: /* child */
            for (int row = 0; row < (rows / 2); row++) {
                for (int col = 0; col < cols; col++) {
                    proc_sum += arr[row][col];
                }
            }
            /* lock and unlock fd around write to prevent both processes writing
             * at the same time */
            flock(fd, LOCK_EX);
            write(fd, &proc_sum, sizeof(cp_input));
            flock(fd, LOCK_UN);
            exit(0);
        default:  /* parent */
            parent_pid = getpid();
            for (int row = (rows / 2); row < rows; row++) {
                for (int col = 0; col < cols; col++) {
                    proc_sum += arr[row][col];
                }
            }
            /* lock and unlock fd around write to prevent both processes writing
             * at the same time */
            flock(fd, LOCK_EX);
            write(fd, &proc_sum, sizeof(cp_input));
            flock(fd, LOCK_UN);
            wait(NULL);
            
    }
    end_timer(&timer);

    printf("child pid: %d\n", child_pid);
    printf("parent pid: %d\n", parent_pid);
    lseek(fd, 0, SEEK_SET);
  
    ssize_t bytes_read;
    char read_bytes[50];
    int posit = 0;
    long res_sum = 0;
    
    /* read each processes sum and sum those two numbers here */
    while ((bytes_read = read(fd, read_bytes, 8)) != 0) {
        res_sum += *(long *)read_bytes;
    }

    printf("Final sum: %ld\n", res_sum);

    /* free dynaic memory */
    for (int row = 0; row < rows; row++)
        free(arr[row]);
    free(arr);

    ret_data_t the_data;
    the_data.elapse_sec = timer.sec_data;
    the_data.elapse_milli = timer.milli_data;
    
    return the_data;    
}