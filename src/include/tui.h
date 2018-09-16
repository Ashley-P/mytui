#ifndef TUI_H_
#define TUI_H_
#include <windows.h>
#include "utils.h"
#include "widgets.h"
#include "const.h"

int tui_init(const int n_screenwidth, const int n_screenheight);
void find_widget(sStack *stack, sWidget *a, const int x, const int y);
void inpthr_loop();
void tui_loop();

#endif
