#include "../../wrappers/wrappers.h"
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

/* eval - evaluate a command line */
void eval(char *cmdline) {
    char *argv[MAXARGS];    /* argument list execve() */
    char buf[MAXLINE];      /* holds modified cmd line */
    int bg;                 /* should teh job run in bg or fg? */
    pid_t pid;              /* process id */

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);
    if (argv[0] == NULL)
        return;             /* ignore lines */

    if (!builtin_command(argv)) {
        if ((pid = Fork()) == 0) { /* child runs user job */
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        } 

        /* parent waits for foreground job to terminate */
        if (!bg) {
            int status;
            if (waitpid(pid, &status, 0) < 0)
                unix_error("waitfg: waitpid error");
        }
        else    
            printf("%d %s", pid, cmdline);
    }
    return;
}

/* if first arg is a builtin command, run it and return true*/
int builtin_command(char **argv) {
    if (!strcmp(argv[0], "quit"))   /* quite command */
        exit(0);
    if (!strcmp(argv[0], "&"))      /* ignore singleton & */
        return 1;
    
    return 0;                       /* not a builtin command */
}

/* parseline - parse the command line and build the argv array */
int parseline(char *buf, char **argv) {
    char *delim;        /* points to first space delimiter */
    int argc;           /* number of args */
    int bg;             /* backround job? */

    buf[strlen(buf) - 1] = ' ';     /* replace trailing \n with space */
    while (*buf && (*buf == ' '))   /* ignore leading spaces */
        buf++;

    /* build argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* ignore spaces */
            buf++;
    }
    argv[argc] = NULL;

    if (argc == 0) /* ignore blank lines */
        return 1;
    
    /* should the job run in the background? */
    if ((bg = (*argv[argc - 1] == '&')) != 0)
        argv[--argc] = NULL;

    return bg;
}