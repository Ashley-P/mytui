#include <stdlib.h>
#include "widgets.h"
#include "draw.h"

sFrame * tui_frame(sWidget *parent) {
    // sFrame setup
    sFrame *ptr = (sFrame *)malloc(sizeof(sFrame));
    for(int i = 0; i < 16; i++)
        ptr->children[i] = NULL;

    // sWidget setup
    sWidget *ptr2      = (sWidget *)malloc(sizeof(sWidget));
    ptr2->type         = FRAME;
    ptr2->min_width    = 0;
    ptr2->min_height   = 0;
    ptr2->parent       = parent;
    ptr2->widget.frame = ptr;

    return ptr;
}

sButton * tui_button(sWidget *parent, wchar_t *text, void(*callback)()) {
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

sMinSize calculate_min_size(sWidget *widget) {
    // Empty struct is used so it can be returned later
    sMinSize s_return;
    switch(widget->type) {
        case FRAME:
            break;
        case BUTTON:
            break;
    }
}

// Used to return 
sMinSize max_sMinSize(sMinSize a, sMinSize b) {
    // Empty struct is used so it can be returned later
    sMinSize s_return;
    s_return width = a.width > b.width ? a.width : b.width;
    s_return height = a.height > b.height ? a.height : b.height;
    return s_return;
}
