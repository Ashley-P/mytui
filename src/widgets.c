#include <stdlib.h>
#include "widgets.h"
#include "draw.h"
#include "utils.h"


sWidget * tui_frame(sWidget *parent) {
    // sWidget setup
    sWidget *ptr            = (sWidget *)malloc(sizeof(sWidget));
    ptr->type               = FRAME;
    ptr->minsize.x      = 0;
    ptr->minsize.y     = 0;
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
    ptr->minsize.x  = 1;
    ptr->minsize.y = 1;
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

sSize calculate_min_size(sWidget *widget) {
    // Empty struct is used so it can be returned later
    sSize s_return = {0, 0};
    sSize s_temp = {0, 0};
    switch(widget->type) {
        case FRAME:
            // Calculate y with the formula (2 + minimum of miny of each row)
            // Calculate x with the formula (2 + minimum of minx of each row)
            // Margins can get integrated into the formula later
            /* y */
            for(int i = 0; i < MAX_GRID_COLS; i++) {
                for(int j = 0; j < MAX_GRID_ROWS; j++) {
                    if (widget->widget.frame.grid[i][j])
                        s_temp = add_sSize(calculate_min_size(widget->widget.frame.grid[i][j]), s_temp);
                }
                widget->widget.frame.cols_size[i].y = s_temp.y;
                s_temp.x = 0;
                s_return = max_sSize(s_return, s_temp);
                s_return.y += 2;
            }

            /* x */
            for(int k = 0; k < MAX_GRID_ROWS; k++) {
                for(int l = 0; l < MAX_GRID_COLS; l++) {
                    if (widget->widget.frame.grid[l][k])
                        s_temp = add_sSize(calculate_min_size(widget->widget.frame.grid[l][k]), s_temp);
                }
                widget->widget.frame.rows_size[k].x = s_temp.x;
                s_temp.y = 0;
                s_return = max_sSize(s_return, s_temp);
                s_return.x += 2;
            }
            break;
        case BUTTON:
            s_return.y = 1;
            s_return.x  = wcslen(widget->widget.button.text);
            break;
        default:
            s_return.y = 0;
            s_return.x  = 0;
            break;
    }
    return s_return;
}

sSize add_sSize(sSize a, sSize b) {
    sSize s_return;
    s_return.x = a.x + b.x;
    s_return.y = a.y = b.y;
    return s_return;
}

// Used to return a sSize struct with the largest of each member
sSize max_sSize(sSize a, sSize b) {
    sSize s_return;
    s_return.x = a.x > b.x ? a.x : b.x;
    s_return.y = a.y > b.y ? a.y : b.y;
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
