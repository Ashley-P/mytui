#include <stdlib.h>
#include "widgets.h"
#include "draw.h"

sButton * tui_button(const sFrame *parent, wchar_t *text, void(*callback)()) {
    sButton *ptr = (sButton *)malloc(sizeof(sButton));
    ptr->draw = &draw_button;
    ptr->text = text;
    ptr->callback = callback;


    return ptr;
}

sFrame * tui_frame(const sFrame *parent) {
    sFrame *ptr = (sFrame *)malloc(sizeof(sFrame));
    return ptr;
}
