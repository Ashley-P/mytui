#ifndef DRAW_H_
#define DRAW_H_

#include <stdbool.h>
#include <wchar.h>
#include <windows.h>
#include "widgets.h"

void reset_buf();
void draw_frame(sWidget *a, const bool fill, int colour);
void draw_button(const sWidget *a);

#endif
