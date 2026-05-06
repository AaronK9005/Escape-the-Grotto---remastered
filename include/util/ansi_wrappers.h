#pragma once

void ansi_up(int n);
void ansi_down(int n);
void ansi_right(int n);
void ansi_left(int n);

void ansi_save_pos();
void ansi_use_pos();

void ansi_goto_home();
void ansi_goto(int x, int y);
void ansi_goto_rel(int dx, int dy);

int ansi_mvprintf(int x, int y, const char* fmt, ...);

void ansi_clear_screen();
void ansi_clear_line();
void ansi_clear_begl();
void ansi_clear_endl();

void ansi_hide_cursor();
void ansi_show_cursor();

void ansi_reset();

void ansi_bold();
void ansi_faint();
void ansi_italic();
void ansi_underline();
void ansi_blink_slow();
void ansi_blink_fast();
void ansi_reverse();
void ansi_hidden();
void ansi_strikethrough();

void ansi_reset_bold();
void ansi_reset_bold_faint();
void ansi_reset_italic();
void ansi_reset_underline();
void ansi_reset_blink();
void ansi_reset_reverse();
void ansi_reset_hidden();
void ansi_reset_strikethrough();

void ansi_black();
void ansi_red();
void ansi_green();
void ansi_yellow();
void ansi_blue();
void ansi_magenta();
void ansi_cyan();
void ansi_white();
void ansi_default();

void ansi_bright_black();
void ansi_bright_red();
void ansi_bright_green();
void ansi_bright_yellow();
void ansi_bright_blue();
void ansi_bright_magenta();
void ansi_bright_cyan();
void ansi_bright_white();

void ansi_bg_black();
void ansi_bg_red();
void ansi_bg_green();
void ansi_bg_yellow();
void ansi_bg_blue();
void ansi_bg_magenta();
void ansi_bg_cyan();
void ansi_bg_white();
void ansi_bg_default();

void ansi_bg_bright_black();
void ansi_bg_bright_red();
void ansi_bg_bright_green();
void ansi_bg_bright_yellow();
void ansi_bg_bright_blue();
void ansi_bg_bright_magenta();
void ansi_bg_bright_cyan();
void ansi_bg_bright_white();

void ansi_256color_fg(int color);
void ansi_256color_bg(int color);