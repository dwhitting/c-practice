#include <stdio.h>
#include <string.h>

int read_line(char str[], int n);

#define MAX_ENTRIES 30
#define MAX_CHARS 20

int main(void) {
    int entry_num = 0;
    char entries[MAX_ENTRIES][MAX_CHARS];
    char small_word[MAX_CHARS];
    char big_word[MAX_CHARS];

    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (entry_num == 30)
            break;
        read_line(entries[entry_num], MAX_CHARS);
        if (entry_num == 0) {
            strcpy(small_word, entries[entry_num]);
            strcpy(big_word, entries[entry_num]);
        }
        if (strcmp(entries[entry_num], small_word) < 0)
            strcpy(small_word, entries[entry_num]);
        if (strcmp(entries[entry_num], big_word) > 0)
            strcpy(big_word, entries[entry_num]);

        entry_num++;
        
        if (strlen(entries[entry_num - 1]) == 4)
            break;
    }

    printf("Smallest word: %s\n", small_word);
    printf("Biggest word: %s\n", big_word);

    //for (int i = 0; i < entry_num; i++)
    //    printf("%s\n", entries[i]);

    return 0;
}

int read_line(char str[], int n) {
    int ch, i = 0;
    while ((ch = getchar()) != '\n')
        if (i < n)
            str[i++] = ch;
    str[i] = '\0';
    return i;
}
