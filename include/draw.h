#ifndef DRAW_H_
#define DRAW_H_

#include <stdbool.h>
#include <wchar.h>
#include <windows.h>
#include "widgets.h"

void reset_buf();

void draw_box(int x, int y, const int x, const int y, const bool fill);

void draw_str(const wchar_t *str, int x, int y);

void draw_button(sButton * button);

#endif
