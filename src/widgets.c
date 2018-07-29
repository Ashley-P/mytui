#include <stdlib.h>
#include "widgets.h"
#include "draw.h"

sFrame * tui_frame(sFrame *parent) {
    // sFrame setup
    sFrame *ptr = (sFrame *)malloc(sizeof(sFrame));

    // sWidget setup
    sWidget *ptr2      = (sWidget *)malloc(sizeof(sWidget));
    ptr2->type         = FRAME;
    ptr2->min_width    = 0;
    ptr2->min_height   = 0;
    ptr2->parent       = parent;
    ptr2->widget.frame = ptr;

    return ptr;
}

sButton * tui_button(sFrame *parent, wchar_t *text, void(*callback)()) {
    // sButton setup
    sButton *ptr  = (sButton *)malloc(sizeof(sButton));
    ptr->draw     = &draw_button;
    ptr->text     = text;
    ptr->callback = callback;

    // sWidget setup
    sWidget *ptr2       = (sWidget *)malloc(sizeof(sWidget));
    ptr2->type          = BUTTON;
    ptr2->min_width     = 1;
    ptr2->min_height    = 1;
    ptr2->parent        = parent;
    ptr2->widget.button = ptr;

    return ptr;
}

