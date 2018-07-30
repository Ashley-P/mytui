#include <stdlib.h>
#include "widgets.h"
#include "draw.h"
#include "utils.h"


sFrame * tui_frame(sWidget *parent) {
    // sFrame setup
    sFrame *ptr = (sFrame *)malloc(sizeof(sFrame));
    for(int i = 0; i < 16; i++)
        ptr->children[i] = NULL;

    // sWidget setup
    sWidget *ptr2        = (sWidget *)malloc(sizeof(sWidget));
    ptr2->type           = FRAME;
    ptr2->parent         = parent;
    ptr2->widget.frame   = ptr;
    ptr2->minsize.width  = 0;
    ptr2->minsize.height = 0;

    return ptr;
}

sButton * tui_button(sWidget *parent, wchar_t *text, void(*callback)()) {
    // sButton setup
    sButton *ptr  = (sButton *)malloc(sizeof(sButton));
    ptr->draw     = &draw_button;
    ptr->text     = text;
    ptr->callback = callback;

    // sWidget setup
    sWidget *ptr2        = (sWidget *)malloc(sizeof(sWidget));
    ptr2->type           = BUTTON;
    ptr2->parent         = parent;
    ptr2->widget.button  = ptr;
    ptr2->minsize.width  = 1;
    ptr2->minsize.height = 1;

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
        default:
            tui_err("calculate_min_size switch default", 0, 0);
            break;
    }
    return s_return;
}

// Used to return 
sMinSize max_sMinSize(sMinSize a, sMinSize b) {
    // Empty struct is used so it can be returned later
    sMinSize s_return;
    s_return.width = a.width > b.width ? a.width : b.width;
    s_return.height = a.height > b.height ? a.height : b.height;
    return s_return;
}
