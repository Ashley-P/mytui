#include <stdlib.h>
#include "widgets.h"
#include "draw.h"
#include "utils.h"


sWidget * tui_frame(sWidget *parent) {
    // sWidget setup
    sWidget *ptr            = (sWidget *)malloc(sizeof(sWidget));
    ptr->type               = FRAME;
    ptr->minsize.width      = 0;
    ptr->minsize.height     = 0;
    ptr->widget.frame.numch = 0;
    if (parent_widget_type(parent)) {
        ptr->parent = parent;
        /* Adding the ptr to the array of children and using the counter as a position indicator */
        switch(parent->type) {
            case FRAME:
                parent->widget.frame.children[parent->widget.frame.numch] = ptr;
                parent->widget.frame.numch += 1;
                break;
            default:
                break;
        }
    }

    // sFrame setup
    for(int i = 0; i < 16; i++)
        ptr->widget.frame.children[i] = NULL;

    return ptr;
}

sWidget * tui_button(sWidget *parent, wchar_t *text, void(*callback)()) {
    // sWidget setup
    sWidget *ptr        = (sWidget *)malloc(sizeof(sWidget));
    ptr->type           = BUTTON;
    ptr->minsize.width  = 1;
    ptr->minsize.height = 1;
    if (parent_widget_type(parent)) {
        ptr->parent = parent;
        /* Adding the ptr to the array of children and using the counter as a position indicator */
        switch(parent->type) {
            case FRAME:
                parent->widget.frame.children[parent->widget.frame.numch] = ptr;
                parent->widget.frame.numch += 1;
                break;
            default:
                break;
        }
    }

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
            // Calculate height with the formula (2 + minimum of min height of each row)
            // Calculate width with the formula (2 + minimum of minwidth of each row)
            // Margins can get integrated into the formula later
            // Set up gridding before you get to this
            break;
        case BUTTON:
            s_return.height = 1;
            s_return.width  = wcslen(widget->widget.button.text);
            break;
        default:
            s_return.height = 0;
            s_return.width  = 0;
            break;
    }
    return s_return;
}

// Used to return a sMinSize struct with the largest of each member
sMinSize max_sMinSize(sMinSize a, sMinSize b) {
    // Empty struct is used so it can be returned later
    sMinSize s_return;
    s_return.width = a.width > b.width ? a.width : b.width;
    s_return.height = a.height > b.height ? a.height : b.height;
    return s_return;
}

int parent_widget_type(sWidget *widget) {
    /*
     * This function checks if the widget passed is of a type that can have children 
     * This is so the user doesn't make the wrong type of widget
     * a parent
     */
    switch(widget->type) {
        case FRAME:
            break;
        case BUTTON:
            tui_err("Wrong widget type; got sButton", 1, 1);
            break;
        default:
            break;
    }

    return 1;
}
