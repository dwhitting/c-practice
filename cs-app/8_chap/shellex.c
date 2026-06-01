#include "../wrappers/wrappers.h"
#define MAXARGS 128

/* function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);

int main(void) 
{
    char cmdline[MAXLINE]; /* command line */

    while (1) {
        /* read */
        printf("> ");
        Fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin))
            return 0;
        
        /* evaluate */
        eval(cmdline);
    }
}