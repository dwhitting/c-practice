#include "../my_timer/my_timer.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    int rows = 10000;
    int cols = 10000;

    char template[] = "/tmp/tempFileXXXXXX";
    long cp_input = 0;
    long read_string = 0;
    long proc_sum = 0;
    my_timer_t timer;

    setbuf(stdout, NULL);
    
    int fd = mkstemp(template);
    printf("file name: %s\n", template);

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

    pid_t child_pid, parent_pid;

    begin_timer(&timer);
    switch (child_pid = fork()) {
        
        case 0:
            for (int row = 0; row < (rows / 2); row++) {
                for (int col = 0; col < cols; col++) {
                    proc_sum += arr[row][col];
                }
            }
            write(fd, &proc_sum, sizeof(cp_input));
            exit(0);
        default:  /* parent */
            parent_pid = getpid();
            for (int row = (rows / 2); row < rows; row++) {
                for (int col = 0; col < cols; col++) {
                    proc_sum += arr[row][col];
                }
            }
            write(fd, &proc_sum, sizeof(cp_input));
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
    
    while ((bytes_read = read(fd, read_bytes, 8)) != 0) {
        res_sum += *(long *)read_bytes;
    }

    printf("Final sum: %ld\n", res_sum);
    
    return 0;    
}