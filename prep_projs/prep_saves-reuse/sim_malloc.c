#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define GB (1UL << 30)

int main(void) 
{
    size_t alloc_size = GB;

    int *arr = mmap(0, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (arr == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    pid_t pid; 
    if ((pid = fork()) == -1) {
        perror("for error");
        exit(1);
    } else if (pid == 0) {
        printf("in child\n");
        arr[3] = 5;
        for (int i = 0; i < 5; i++) {
            printf("%d\n", arr[i]);
        }
        _exit(0);
    }

    int stat;
    wait(&stat);

    printf("after fork, stat: %d\n", stat);
    printf("This pid is %d\n", getpid());

    if (WIFEXITED(stat)) {
        int exit_status = WEXITSTATUS(stat);
        printf("Proc <%d> ended with status: %d\n", pid, exit_status);
    }

    size_t div_mem = alloc_size / 4;
    printf("divmem: %lu\n", div_mem);

    arr[268435455] = 3;
    for (int i = 0; i < 5; i++) {
        printf("%d\n", arr[i]);
    }

    printf("far el: %d\n", arr[3]);

    printf("size: %lu\n", sizeof(arr));

    
    if (munmap(arr, alloc_size) == -1){
        perror("unmap error");
        exit(1);
    }


    return 0;
}
