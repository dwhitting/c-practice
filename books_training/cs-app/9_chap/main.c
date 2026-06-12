#include <stdio.h>
#include "../wrappers/wrappers.h"

static char *mem_heap;      /* points to first byte of heap */
static char *mem_brk;       /* points to last byte of heap plus 1*/
static char *mem_max_addr;  /* max legal heap addr plus 1*/

#define MAX_HEAP 4000000

/* initialize mem model */
void mem_init(void) {
    mem_heap = (char *) Malloc(MAX_HEAP);
    mem_brk = (char *) mem_heap;
    mem_max_addr = (char *) (mem_heap + MAX_HEAP);
}

/* simple model of sbrk function */
void *mem_sbrk(int incr) {
    char *old_brk = mem_brk;

    if ((incr < 0) || ((mem_brk + incr) > mem_max_addr)) {
        errno = ENOMEM;
        fprintf(stderr, "ERROR: mem_sbrk failed. Ran out of memory...\n");
        return (void *) -1;
    }
    mem_brk += incr;
    return (void *) old_brk;
}

int main(void)
{
    mem_init();
    mem_sbrk(100);
    

    return 0;
}