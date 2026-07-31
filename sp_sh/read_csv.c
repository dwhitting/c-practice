#include "stan_hdr.h"

int main(void) {

    int fd = open("budj_est.csv", O_RDONLY);

    if (fd < 0) {
        printf("error opening csv\n");
        exit(0);
    }

    char csv_read[NOTE_LEN];

    

    strcpy(csv_read, "");
    char char_read;

    int times = 10;

    for (int n = 0; n < times; n++) {
        char_read = '\0';

        while (char_read != '\n') {
            if (read(fd, &char_read, sizeof(char)) <= 0) {
                printf("Error reading single char\n");
                exit(0);
            }
            printf("%c", char_read);
        }

        printf("\n");    
    }

    close(fd);

    return 0;
}