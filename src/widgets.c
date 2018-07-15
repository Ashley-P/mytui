#include <stdlib.h>
#include "widgets.h"
#include "draw.h"

sButton * tui_button(wchar_t *text, void(*callback)()) {
    sButton *ptr = (sButton *)malloc(sizeof(sButton));
    ptr->draw = &draw_button;
    ptr->text = text;
    if (callback != NULL)
        ptr->callback = callback;
    else
        ptr->callback = NULL;


    return ptr;
}

sFrame * tui_frame(const sFrame *master) {
    sFrame *ptr = (sFrame *)malloc(sizeof(sFrame));
    return ptr;
}
