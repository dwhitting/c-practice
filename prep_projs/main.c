#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../hl_projs/my_timer/my_timer.h"

#define N 1024
#define BLOCK_SIZE 64

/* 1024 took 36 sec, 722 ms */

int matrix_multiply_unblocked(int A[N][N], int B[N][N], int C[N][N]);
int matrix_mulitply_blocked(int A[N][N], int B[N][N], int C[N][N]);


int main(void) 
{
    my_timer_t timer;

    int (*A)[N] = malloc(N * sizeof(*A));
    if (A == NULL) {
        perror("A malloc fail");
        exit(1);
    }
    int (*B)[N] = malloc(N * sizeof(*B));
    if (B == NULL) {
        perror("A malloc fail");
        exit(1);
    }
    int (*C)[N] = malloc(N * sizeof(*C));
    if (C == NULL) {
        perror("A malloc fail");
        exit(1);
    }

    begin_timer(&timer);

    matrix_mulitply_blocked(A, B, C);

    end_timer(&timer);

    free(C);
    free(B);
    free(A);

    return 0;
}

int matrix_multiply_unblocked(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
             for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
             }
        }
    }


    return 0;
}

int matrix_mulitply_blocked(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i = 0; i < N; i += BLOCK_SIZE) {
        for (int j = 0; j < N; j += BLOCK_SIZE) {
            for (int k = 0; k < N; k += BLOCK_SIZE) {
                for (int i_b = i; i_b < i + BLOCK_SIZE; i_b++) {
                    for (int k_b = k; k_b < k + BLOCK_SIZE; k_b++) {
                        for (int j_b = j; j_b < j + BLOCK_SIZE; j_b++) {
                            C[i_b][j_b] += A[i_b][k_b] * B[k_b][j_b];
                        }
                    }
                }
            }
        }
    }

    printf("read from shm: %d\n", arr[995]);

    if (munmap(ptr, SHM_SIZE) == -1)
        perror("munmap failed");

    close(shm_fd);
    // if (shm_unlink("/my_shm_region") == -1)
    //     perror("shm unlink failed");

    return 0;   
}
