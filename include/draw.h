#ifndef DRAW_H_
#define DRAW_H_

#include <stdbool.h>
#include <wchar.h>

void reset_buf(wchar_t *buf);

void draw_box(wchar_t *buf, int x, int y, const int width, const int height, const bool fill);

void draw_str(wchar_t *buf, const wchar_t *str, int x, int y);

#endif
