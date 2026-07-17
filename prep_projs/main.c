#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    int one;
    char strng[1024];
} the_struct_t;


int main() {

    the_struct_t *one = malloc(100000 * sizeof(the_struct_t));
    strcpy(one[100000 - 1].strng, "Yo");

   printf("Test %s\n", one[100000 - 1].strng);
   printf("Size: %lu\n", sizeof(*one));

   free(one);
    
    return 0;
}