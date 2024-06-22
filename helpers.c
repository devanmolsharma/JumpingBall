#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

char getKeyPressed()
{
    struct termios oldt, newt;
    int oldf;
    char pressed_key;

    // Get the current terminal settings
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Set the new terminal settings to raw mode
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    // Get the current file descriptor flags
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (oldf == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    // Set the file descriptor to non-blocking mode
    if (fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    // Try to read the key press
    pressed_key = getchar();

    // Restore the file descriptor flags
    if (fcntl(STDIN_FILENO, F_SETFL, oldf) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    // Restore the old terminal settings
    if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    // If no key was pressed, return 0
    if (pressed_key == EOF) {
        return 0;
    }

    return pressed_key;
}



void clearScr()
{
    system("clear");
}