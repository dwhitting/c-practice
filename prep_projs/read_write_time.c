#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include "../hl_projs/my_timer/my_timer.h"

int mkstemp_write(char *file_name);
int mkstemp_read(char *file_name);
int trad_file_write(void);
int trade_file_read(void);
int shm_write(void);
int shm_read(void);

int main(void) 
{
    my_timer_t timer;
    begin_timer(&timer);

    shm_write();
    shm_read();    

    // trad_file_write();
    // trade_file_read();

    // char temp_file[100];
    // mkstemp_write(temp_file);
    // mkstemp_read(temp_file);

    end_timer(&timer);

    return 0;
}

int mkstemp_write(char *file_name) {
    char temp_file[] = "/tmp/my_tempXXXXXX";

    int fd = mkstemp(temp_file);
    if (fd == -1) {
        perror("failed mktemp open");
        exit(1);
    }

    strcpy(file_name, temp_file);
    
    for (int i = 0; i < 1000; i++) {
        write(fd, &i, sizeof(int));
    }    
    
    close(fd);
    return 0;
}

int mkstemp_read(char *file_name) {
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        perror("open file for read error");
        exit(1);
    }

    int arr[1000], cntr = 0, read_num;
    while (read(fd, &read_num, sizeof(int)) != 0) {
        arr[cntr] = cntr;
        cntr++;
    }

    printf("output: %d\n", arr[995]);

    unlink(file_name);
    close(fd);

    return 0;
}

int trad_file_write(void) {
    char *path = "./data";
    int fd;
    if ((fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1) {
        perror("file open error");
        exit(1);
    }

    for (int i = 0; i < 1000; i++) {
        write(fd, &i, sizeof(int));
    }
    
    close(fd);

    return 0;
}

int trade_file_read(void) {
    char *path = "./data";
    int fd;
    if ((fd = open(path, O_RDONLY)) == -1) {
        perror("file open error");
        exit(1);
    }

    int read_num;
    int arr[1000], cntr = 0;
    while (read(fd, &read_num, sizeof(int)) != 0) {
        arr[cntr] = cntr;
        cntr++;
    }

    printf("%d\n", arr[955]);

    close(fd);

    return 0;
}

int shm_write(void) {
    const char *name = "/my_shm_region";
    const unsigned long SHM_SIZE = 4096;

    int shm_fd = shm_open(name, O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (shm_fd < 0) {
        perror("shm did not open");
        exit(1);
    }
    ftruncate(shm_fd, SHM_SIZE);
    void *ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    int arr[1000];
    for (int i = 0; i < 1000; i++) {
        arr[i] = i;
    }

    char *base_ptr = (char *)ptr;
    unsigned long offset = 0;
    for (int i = 0; i < 1000; i++) {
        if (offset + sizeof(int) <= SHM_SIZE) {
            memcpy(base_ptr + offset, &arr[i], sizeof(int));
            offset += sizeof(int);
        }
    }
    
    printf("Wrote to shm\n");
 

    munmap(ptr, SHM_SIZE);
    close(shm_fd);

    return 0;   

    /* to check for orphaned shm: ls -l /dev/shm */
}

int shm_read(void) {
    const char *name = "/my_shm_region";
    const unsigned long SHM_SIZE = 4096;

    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm did not open");
        exit(1);
    }
    ftruncate(shm_fd, SHM_SIZE);
    void *ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    int arr[1000];
    char *base_ptr = (char *)ptr;
    unsigned long offset = 0;
    for (int i = 0; i < 1000; i++) {
        if (offset + sizeof(int) <= SHM_SIZE) {
            memcpy(&arr[i], base_ptr + offset, sizeof(int));
            offset += sizeof(int);
        }
    }


    printf("read from shm: %d\n", arr[995]);

    munmap(ptr, SHM_SIZE);
    close(shm_fd);

    return 0;   
}