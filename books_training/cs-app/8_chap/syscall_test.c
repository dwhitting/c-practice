#include <unistd.h>
#include <sys/syscall.h>

int main(void)
{
    char msg[] = "Hello, Kernel!\n";

    syscall(SYS_write, 1, msg, 15);

    syscall(SYS_exit, 0);

}