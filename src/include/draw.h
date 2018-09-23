#ifndef DRAW_H_
#define DRAW_H_

#include <stdbool.h>
#include <wchar.h>
#include <windows.h>
#include "widgets.h"

void reset_buf();
void draw_frame(sWidget *a, const bool fill);
void draw_button(const sWidget *a);
void draw_label(const sWidget *a);
void draw_checkbox(const sWidget *a);

#endif
