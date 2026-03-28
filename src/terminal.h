#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>
#include <unistd.h>

/* 端末設定を変更する（バッファとエコーを無効化） */
void TerminalSet(struct termios *const oldt, struct termios *const newt);

/* 端末設定を元に戻す */
void TerminalReset(const struct termios *const oldt);

#endif
