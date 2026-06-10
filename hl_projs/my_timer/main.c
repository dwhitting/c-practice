#include "my_timer.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    char template[] = "/tmp/tempFileXXXXXX";
    long cp_input = 0;
    long read_string = 0;

    setbuf(stdout, NULL);
    
    int fd = mkstemp(template);
    printf("file name: %s\n", template);

    pid_t child_pid, parent_pid;

    switch (child_pid = fork()) {
        
        case 0:
            cp_input = 5;
            write(fd, cp_input, sizeof(cp_input));
            exit(0);
        default:  /* parent */
            parent_pid = getpid();
            cp_input = 6;
            write(fd, cp_input, sizeof(cp_input));
            wait(NULL);
            
    }
    printf("child pid: %d\n", child_pid);
    printf("parent pid: %d\n", parent_pid);
    lseek(fd, 0, SEEK_SET);
  
    ssize_t bytes_read;
    //char *read_ptr = read_string;
    int posit = 0;
    
    while ((bytes_read = read(fd, cp_input, sizeof(cp_input) - 1)) != 0) {
        //cp_input[bytes_read] = '\0';
        //memcpy(read_ptr, cp_input, bytes_read);
        //read_ptr += bytes_read;
        printf("while: %ld\n", cp_input);
    }
    
    //*read_ptr = '\0';
    printf("bytes: %ld\n", strlen(read_string));
    printf("string: %d\n", read_string);
    return 0;    
}