#ifndef DRAW_H_
#define DRAW_H_

#include <stdbool.h>
#include <wchar.h>
#include <windows.h>
#include "widgets.h"

void reset_buf(CHAR_INFO *buf);

void draw_box(CHAR_INFO *buf, int x, int y, const int width, const int height, const bool fill);

void draw_str(CHAR_INFO *buf, const wchar_t *str, int x, int y);

void draw_button(CHAR_INFO *buf, sButton * button);

#endif
