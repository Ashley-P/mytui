#ifndef DRAW_H_
#define DRAW_H_

#include <wchar.h>
#include <windows.h>
#include "widgets.h"

void reset_buf(CHAR_INFO *arr, const size_t len);
void draw_line(const int x, const int y, const int len, const int direction, const unsigned char colour);
void draw_box(int x, int y, const int width, const int height, const int fill, const unsigned char colour);
void draw_str(const wchar_t *str, const size_t len, const int x, const int y, const size_t line_break_len,
        const unsigned char colour);
void draw_frame(const sWidget *a, const int fill);
void draw_button(const sWidget *a);
void draw_label(const sWidget *a);
void draw_checkbox(const sWidget *a);
void draw_radiobutton(const sWidget *a);
void draw_canvas(const sWidget *a);
void draw_field(sWidget *a);

#endif
