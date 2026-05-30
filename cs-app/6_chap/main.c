#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define N 1000
#define M 1000

int main(void)
{
    // for high precision
    struct timespec ts_1, ts_2;
    timespec_get(&ts_1, TIME_UTC);

    struct tm *t_1 = localtime(&ts_1.tv_sec);
    long millisec = ts_1.tv_nsec / 1000000;
    
    // int rand_num = rand();
    // printf("rnd: %d\n", rand_num);

    //int arr[N][N];
    //int (*arr_heap)[M] = malloc(N * sizeof(*arr_heap)); // N rows x M cols; one continiuous block on heap
    
    int **arr_heap = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
        arr_heap[i] = malloc(M * sizeof(int));


    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            arr_heap[i][j] = (rand() % 100) + 1;

    int sum = 0;

    //printf("output, min: %d, sec: %d\n",t->tm_min, t->tm_sec);
    printf("sec and millisec 1: %d, %ld\n",t_1->tm_sec, millisec);

    // struct timespec delay = { .tv_sec = 0, .tv_nsec = 250000000}; //250 millisec
    // nanosleep(&delay, NULL);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            sum += arr_heap[i][j];

    timespec_get(&ts_2, TIME_UTC);
    struct tm *t_2 = localtime(&ts_2.tv_sec);
    long second_millisec = ts_2.tv_nsec / 1000000;
    printf("sec and millisec 2: %d, %ld\n", t_2->tm_sec, second_millisec);

    printf("sum: %d\n", sum);

    for (int i = 0; i < N; i++)
        free(arr_heap[i]);
    free(arr_heap);

    return 0;
}