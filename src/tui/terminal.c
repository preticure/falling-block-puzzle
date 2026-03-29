#include "terminal.h"

void TerminalSet(struct termios *const oldt, struct termios *const newt)
{
    tcgetattr(STDIN_FILENO, oldt);
    *newt = *oldt;
    newt->c_lflag &= ~(ICANON | ECHO); /* バッファとエコーを無効化 */
    tcsetattr(STDIN_FILENO, TCSANOW, newt);
}

void TerminalReset(const struct termios *const oldt)
{
    tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}
