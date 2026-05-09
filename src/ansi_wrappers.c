#include "util/ansi_wrappers.h"

#include <stdarg.h>

int printf(const char*, ...);
int putchar(int);
int vprintf(const char *format, va_list ap);

void ansi_up(int n) { printf("\x1b[%dA", n); }
void ansi_down(int n)  { printf("\x1b[%dB", n); }
void ansi_right(int n) { printf("\x1b[%dC", n); }
void ansi_left(int n)  { printf("\x1b[%dD", n); }

void ansi_save_pos() { printf("\x1b[s"); }
void ansi_use_pos() { printf("\x1b[u"); }

void ansi_goto_home() { printf("\x1b[H"); }
void ansi_goto(int x, int y) { printf("\x1b[%d;%dH", y, x); }
void ansi_goto_rel(int dx, int dy) {
    if (dy < 0) printf("\x1b[%dA", -dy);  // up
    if (dy > 0) printf("\x1b[%dB", dy);   // down
    if (dx > 0) printf("\x1b[%dC", dx);   // right
    if (dx < 0) printf("\x1b[%dD", -dx);  // left
}
int ansi_mvprintf(int x, int y, const char* fmt, ...) {
    va_list args;
    int printed = 0;

    ansi_goto(x, y);

    va_start(args, fmt);
    printed = vprintf(fmt, args);
    va_end(args);

    return printed;
}

void ansi_clear_screen() { printf("\x1b[2J"); }
void ansi_clear_line() { printf("\x1b[2K"); }
void ansi_clear_begl() { printf("\x1b[1K");}
void ansi_clear_endl() { printf("\x1b[0K");}

void ansi_hide_cursor() { printf("\x1b[?25l"); }
void ansi_show_cursor() { printf("\x1b[?25h"); }

void ansi_reset() { printf("\x1b[0m"); }

void ansi_bold() { printf("\x1b[1m"); }
void ansi_faint() { printf("\x1b[2m"); }
void ansi_italic() { printf("\x1b[3m"); }
void ansi_underline() { printf("\x1b[4m"); }
void ansi_blink_slow() { printf("\x1b[5m"); }
void ansi_blink_fast() { printf("\x1b[6m"); }
void ansi_reverse() { printf("\x1b[7m"); }
void ansi_hidden() { printf("\x1b[8m"); }
void ansi_strikethrough() { printf("\x1b[9m"); }

void ansi_reset_bold() { printf("\x1b[22m"); }
void ansi_reset_bold_faint() { printf("\x1b[22m"); }
void ansi_reset_italic() { printf("\x1b[23m"); }
void ansi_reset_underline() { printf("\x1b[24m"); }
void ansi_reset_blink() { printf("\x1b[25m"); }
void ansi_reset_reverse() { printf("\x1b[27m"); }
void ansi_reset_hidden() { printf("\x1b[28m"); }
void ansi_reset_strikethrough() { printf("\x1b[29m"); }

void ansi_black() { printf("\x1b[30m"); }
void ansi_red() { printf("\x1b[31m"); }
void ansi_green() { printf("\x1b[32m"); }
void ansi_yellow() { printf("\x1b[33m"); }
void ansi_blue() { printf("\x1b[34m"); }
void ansi_magenta() { printf("\x1b[35m"); }
void ansi_cyan() { printf("\x1b[36m"); }
void ansi_white() { printf("\x1b[37m"); }
void ansi_default() { printf("\x1b[39m"); }

void ansi_bright_black() { printf("\x1b[90m"); }
void ansi_bright_red() { printf("\x1b[91m"); }
void ansi_bright_green() { printf("\x1b[92m"); }
void ansi_bright_yellow() { printf("\x1b[93m"); }
void ansi_bright_blue() { printf("\x1b[94m"); }
void ansi_bright_magenta() { printf("\x1b[95m"); }
void ansi_bright_cyan() { printf("\x1b[96m"); }
void ansi_bright_white() { printf("\x1b[97m"); }

void ansi_bg_black() { printf("\x1b[40m"); }
void ansi_bg_red() { printf("\x1b[41m"); }
void ansi_bg_green() { printf("\x1b[42m"); }
void ansi_bg_yellow() { printf("\x1b[43m"); }
void ansi_bg_blue() { printf("\x1b[44m"); }
void ansi_bg_magenta() { printf("\x1b[45m"); }
void ansi_bg_cyan() { printf("\x1b[46m"); }
void ansi_bg_white() { printf("\x1b[47m"); }
void ansi_bg_default() { printf("\x1b[49m"); }

void ansi_bg_bright_black() { printf("\x1b[100m"); }
void ansi_bg_bright_red() { printf("\x1b[101m"); }
void ansi_bg_bright_green() { printf("\x1b[102m"); }
void ansi_bg_bright_yellow() { printf("\x1b[103m"); }
void ansi_bg_bright_blue() { printf("\x1b[104m"); }
void ansi_bg_bright_magenta() { printf("\x1b[105m"); }
void ansi_bg_bright_cyan() { printf("\x1b[106m"); }
void ansi_bg_bright_white() { printf("\x1b[107m"); }

void ansi_256color_fg(int color) { printf("\x1b[38;5;%dm", color); }
void ansi_256color_bg(int color) { printf("\x1b[48;5;%dm", color); }