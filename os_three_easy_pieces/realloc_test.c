#include <stdio.h>
#include <stdlib.h>

int main(void)
{

    int *arr = (int *) malloc(5 * sizeof(int));
    if (arr == NULL) return 1;

    for (int i = 0; i < 5; i++) {
        arr[i] = i;
    }

    for (int i = 0; i < 5; i++){
        printf("%d, ", arr[i]);
    }
    printf("\n");

    int *temp = realloc(arr, 10 * sizeof(int));
    if (temp == NULL) {
        fprintf(stderr, "failed to realloc\n");
        free(arr);
        return 1;
    }

    arr = temp;
    
    for (int i = 5; i < 10; i++){
        arr[i] = i;
    }

    for (int i = 0; i < 10; i++) {
        printf("%d, ", arr[i]);
    }

    free(arr);

    printf("\n");
    return 0;
}