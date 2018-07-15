#include <stdlib.h>
#include "widgets.h"
#include "draw.h"

sButton * tui_button(const int px, const int py, const int width,
                     const int height, wchar_t *text, void (*callback)()) {
    sButton *ptr = (sButton *)malloc(sizeof(sButton));
    ptr->px = px;
    ptr->py = py;
    ptr->width = width;
    ptr->height = height;
    ptr->text = text;
    ptr->draw = &draw_button;
    if (callback != NULL)
        ptr->callback = callback;
    else
        ptr->callback = NULL;


    return ptr;
}

sFrame * tui_frame(const sFrame *master) {}
