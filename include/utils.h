#ifndef UTILS_H_
#define UTILS_H_

#define TUI_ERROR 1
#define TUI_WARNING 2
#define TUI_OTHER 3

void init_stderr();

void win_err(const char *msg);

void tui_err(const char *msg, const int err_type, const int quit_prog);

#endif
