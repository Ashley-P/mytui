#ifndef INPUT_H_
#define INPUT_H_

#include "widgets.h"

void tui_handle_input();

void frame_mouse_event(sWidget *a, sWidget **old, MOUSE_EVENT_RECORD *ev);

void button_mouse_event(sWidget *a, sWidget **old, MOUSE_EVENT_RECORD *ev);

#endif
