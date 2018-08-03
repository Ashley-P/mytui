#include <stdlib.h>
#include "widgets.h"
#include "draw.h"
#include "utils.h"


sWidget * tui_frame(sWidget *parent) {
    // sWidget setup
    sWidget *ptr        = (sWidget *)malloc(sizeof(sWidget));
    ptr->type           = FRAME;
    ptr->parent         = parent;
    ptr->minsize.width  = 0;
    ptr->minsize.height = 0;

    // sFrame setup
    for(int i = 0; i < 16; i++)
        ptr->widget.frame.children[i] = NULL;

    return ptr;
}

sWidget * tui_button(sWidget *parent, wchar_t *text, void(*callback)()) {
    // sWidget setup
    sWidget *ptr        = (sWidget *)malloc(sizeof(sWidget));
    ptr->type           = BUTTON;
    ptr->parent         = parent;
    ptr->minsize.width  = 1;
    ptr->minsize.height = 1;

    // sButton setup
    ptr->widget.button.text     = text;
    ptr->widget.button.draw     = &draw_button;
    ptr->widget.button.callback = callback;


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
