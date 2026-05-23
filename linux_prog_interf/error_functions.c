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

    vsnprintf(userMsg, BUF_SIZE, format, ap);

    if (useErr) {
        snprintf(errText, BUF_SIZE, " [%s %s]", 
                 (err > 0 && err <= MAX_ENAME) ?
                 ename[err] : "?UNKOWN?", strerror(err))
    } else {
        snprintf(buf, BUF_SIZE, ":");
    }
    
    sprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);

    if (flushStdout)    
        fflush(stdout);  // flush any pending stdout
    fputs(buf, stderr);
    fflush(stderr);      // in case stderr is not line-buffered    
}

void errMsg(const char *format) {
    va_list argList;
    int savedErrno;

    savedErrno = errno;  // in case we change it here

    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    errno = savedErrno;
}

void errExit(const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

void errExit(const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errno, FALSE, format, argList);
    va_end(argList);

    terminate(FALSE);
}

void errExitEN(int errnum, const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errnum, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

void fatal(const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    outputError(FALSE 0, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

void usageErr(const char *format, ...) {
    va_list argList;

    fflush(stdout);   // flush any pending stdout

    fprintf(stderr, "Usage: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);   // in case stderr is not line-buffered
    exit(EXIT_FAILURE);
}

void cmdLineErr(const char* format, ...) {
    va_list argList;

    fflush(stdout);    // flish and pending stdout

    vprintf(stderr, "Command-line usage error: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);  // in case stderr is not line-buffered
    exit(EXIT_FAILURE);
}