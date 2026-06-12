#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int main(void)
{
    struct termios old_settings, new_settings;
    char c;

    printf("Hit single character: ");
    fflush(stdout);

    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

    read(STDIN_FILENO, &c, 1);

    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);

    printf("\nyou typed: %c\n", c);

    return 0;
}