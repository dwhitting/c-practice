#ifndef TLPI_HDR_H
#define TLPI_HDR_H      /*  to prevent accidental double inclusion*/

#include <sys/types.h>  /*  type defs used my many programs */
#include <stdio.h>
#include <stdlib.h>     //  prototypes of commonly used libarary funcs, 
                        //  plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /*  prototypes for may sys calls */
#include <errno.h>      /*  delcares errno and defines error constants */
#include <string.h>

#include "get_num.h"    //  declares our functions for handling numeric args
                        //  get(gitInt(), getLong())

#include "error_functions.h" // declares our error-handling functions

typedef enum { FALSE, TRUE } Boolean;

#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))

#endif
