#include <stdlib.h>

#define WSIZE 4             /* word and header/footer size (bytes) */
#define DSIZE 8             /* double word size (bytes) */
#define CHUNKSIZE (1<<12)   /* extend heap by this ammount (byes) 4K */

static char *heap_listp = 0; /* Points to the prologue block */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* pack a size and allocated bit inot a word */
#define PACK(size, alloc) ((size) | (alloc))

/* reaad and write a word at address p */
#define GET(p) (*(unsigned int *) (p))
#define PUT(p, val) (*(unsigned int *) (p) = (val))

/* read teh size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* fiven block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *) (bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *) (bp) + GET_SIZE(((char *) (bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *) (bp) - GET_SIZE(((char *) (bp) - DSIZE)))

int mm_init(void) {
    /* create the initial empty heap */
    if ((heap_listp = memsbrk(4 * WSIZE)) == (void *) -1)
        return -1;
    PUT(heap_listp, 0);                                 /* alignment padding */
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));      /* prologue header */
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));      /* prologue footer */
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));          /* epilogue header */
    heap_listp += (2 * WSIZE);

    /* extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;
    return 0;
}