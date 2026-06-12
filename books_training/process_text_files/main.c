#include <stdio.h>

int main(int args, char *argv[])
{
    if (args < 2) {
        fprintf(stderr, "usage: wfreq file1 [file2]\n");
        return 1;
    }

    FILE *fptr;
    char buff[255];

    int ch;
    int bytecnt;
    int linecnt;

    for (int i = 1; i < args; i++) {
        bytecnt = 0;
        linecnt = 1;
        fptr = fopen(argv[i], "r");

        if (fptr == NULL) {
            printf("Error: could not open file\n");
            //fclose(fptr);
            return 1;
        }

        while ((ch = fgetc(fptr)) != EOF) {
            bytecnt++;
            if (ch == '\n') linecnt++;
        }
        printf("For %s\n", argv[i]);
        printf("bytes: %d\n", bytecnt);
        printf("lines: %d\n", linecnt);
        printf("\n");
        fclose(fptr);
    }



    // while (fgets(buff, 255, fptr)) {
    //     printf("%s", buff);
    // }

    return 0;
}