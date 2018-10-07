#ifndef DRAW_H_
#define DRAW_H_

#include <wchar.h>
#include <windows.h>
#include "widgets.h"

void reset_buf();
void draw_frame(sWidget *a, const int fill);
void draw_button(sWidget *a);
void draw_label(sWidget *a);
void draw_checkbox(sWidget *a);
void draw_radiobutton(sWidget *a);

#endif
