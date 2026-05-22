#include <stdarg.h>
#include "error_functions.h"
#include "tlpi_hdr.h" 
#include "ename.c.inc"    // defines enmae and MAX_ENAME

NORETURN
static void terminate(Boolean useExit3) {
    char *s;

    /*  Dump core if EF_DUMPCORE environment variable is defined and
        is a nonempy string; otherwise call exit(3) or _exit(2), 
        depending on the value of 'useExit3 */

    s = getenv("EF_DUMPCORE");

    if (s != NULL && *s != '\0')
        abort();
    else if (useExit3)
        exit(EXIT_FAILURE);
    else    
        _exit(EXIT_FAILURE);
}

static void outputError(Boolean useErr, int err, Boolean flushStdout, const char *format, 
                        va_list app)
{
#define BUF_SIZE 500
    char buff[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

    
}