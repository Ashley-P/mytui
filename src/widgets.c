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

    // sFrame setup which just makes sure each element in both arrays are set to null
    for(int i = 0; i < MAX_CHILDREN; i++)
        ptr->widget.frame.children[i] = NULL;

    for(int j = 0; j < MAX_GRID_COLS; j++) {
        for(int k = 0; k < MAX_GRID_ROWS; k++) {
            ptr->widget.frame.grid[j][k] = NULL;
        }
    }

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
    sMinSize s_return;
    s_return.width = a.width > b.width ? a.width : b.width;
    s_return.height = a.height > b.height ? a.height : b.height;
    return s_return;
}

int parent_widget_type(sWidget *widget) {
    /*
     * This function checks if the widget passed is of a type that can have children 
     * This is so the user doesn't make the wrong type of widget a parent
     */
    switch(widget->type) {
        case BUTTON:
            tui_err("Wrong widget type; got BUTTON", 1, 1);
            break;
        case FRAME: default:
            break;
    }

    return 1;
}

void grid_set(sWidget *widget, int col, int row) {
    /* This function sets the grid position in its parent */
    /* Make sure there is a warning if a grid position gets overwritten */
    /* Basic checking for arguments */
    char err[256];
    if (col >= MAX_GRID_COLS || col < 0) {
       sprintf(err, "Error incorrect col value; Expected 0 < col < %d - Got %d", MAX_GRID_COLS, col);
       tui_err(err, 1, 1);
    }
    if (row >= MAX_GRID_ROWS || row < 0) {
       sprintf(err, "Error incorrect row value; Expected 0 < row < %d - Got %d", MAX_GRID_ROWS, row);
       tui_err(err, 1, 1);
    }

    /* Actually assigning the widget to the grid */
    /* Don't need to call parent_widget_type because it get called on the constructor for the widget */
    switch(widget->parent->type) {
        case FRAME:
            if (widget->parent->widget.frame.grid[col][row])
                tui_err("Grid position is already taken, overwritten anyway", 2, 0);
            widget->parent->widget.frame.grid[col][row] = widget;
            break;
        case BUTTON: default:
            break;

    }
}
