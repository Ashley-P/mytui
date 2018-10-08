#ifndef DRAW_H_
#define DRAW_H_

#include <wchar.h>
#include <windows.h>
#include "widgets.h"

void reset_buf(CHAR_INFO *arr, size_t len);
void draw_line(const int x, const int y, const int len, const int direction, const int colour);
void draw_box(int x, int y, const int width, const int height, const int fill, int colour);
void draw_str(const wchar_t *str, const size_t len, int x, int y);
void draw_frame(sWidget *a, const int fill);
void draw_button(sWidget *a);
void draw_label(sWidget *a);
void draw_checkbox(sWidget *a);
void draw_radiobutton(sWidget *a);
void draw_canvas(sWidget *a);

#endif
