#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include<termios.h>
#include <ctype.h>
#include <stdio.h>
struct termios orig_termios;
void die(const char *s) {
  perror(s);
  exit(1);
}
void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}
void enablerawmode(){
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    // struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    // With all these things I came to realisation that we are basically turning a terminal into a texteditor
    raw.c_iflag &= ~(ICRNL|IXON);
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO|ICANON|ISIG|IEXTEN);
    raw.c_cflag |= (CS8);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}
int main() {
    enablerawmode();
    char c;
    while (1) {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
        if (iscntrl(c)) {
        printf("%d\r\n", c);
        } else {
        printf("%d ('%c')\r\n", c, c);
        }
        if (c == 'q') break;
    }
    return 0;
}