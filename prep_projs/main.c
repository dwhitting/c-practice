#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Attempting to open a new terminal window...\n");

    // Option 1: gnome-terminal (Common in Ubuntu/Debian)
    // The '&' at the end runs it in the background so your C program isn't blocked.
    int result = system("gnome-terminal &");

    // Option 2: If gnome-terminal fails, try xterm (universal fallback)
    if (result != 0) {
        printf("gnome-terminal not found. Trying xterm...\n");
        result = system("xterm &");
    }

    // Option 3: Try konsole (Common in KDE environments)
    if (result != 0) {
        printf("xterm not found. Trying konsole...\n");
        result = system("konsole &");
    }

    if (result == 0) {
        printf("Terminal opened successfully!\n");
    } else {
        printf("Failed to open a terminal. Please ensure gnome-terminal, xterm, or konsole is installed.\n");
    }

    return 0;
}