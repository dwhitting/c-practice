#include <stdlib.h>
#include <stdio.h>

int double_point(void)
{
    int **arr;

    arr = (int **) malloc(sizeof(int *) * 5);
    if (arr == NULL) return 1;

    arr[0] = (int *) malloc(sizeof(int) * 5);
    if (arr[0] == NULL) return 1;

    arr[4] = (int *) malloc(sizeof(int) * 20);
    if (arr[4] == NULL) return 1;

    arr[0][0] = 7;
    arr[4][19] = 33;

    printf("value[0][0]: %d\n", arr[0][0]);
    printf("value [4][19]: %d\n", arr[4][19]);

    free(arr[0]);
    free(arr[4]);
    free(arr);

    return 0;
}

int from_c_programming(void) {
    char *fruit[] = {
        "watermelon",
        "banana",
        "pear",
        "apple"
    };

    for (int x = 0; x < 4; x++) {
        putchar(**(fruit+x));
        putchar('\n');
    }


    return 0;
}