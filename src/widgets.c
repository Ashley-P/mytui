#include <stdlib.h>
#include "tui.h"
#include "widgets.h"
#include "draw.h"
#include "utils.h"


sWidget * tui_frame(sWidget *parent) {
    // sWidget setup
    sWidget *ptr            = (sWidget *)malloc(sizeof(sWidget));
    ptr->type               = FRAME;
    ptr->size.x          = 0;
    ptr->size.y          = 0;
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
    sWidget *ptr     = (sWidget *)malloc(sizeof(sWidget));
    ptr->type        = BUTTON;
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

/* Margins, paddings, borders and other things that can change size are computed here */
void widget_sizer(sWidget *a) {
    switch (a->type) {
        case FRAME:
            /* Tons of loops because I can't program */
            for(int p = 0; p < MAX_CHILDREN; p++) {
                if(a->widget.frame.children[p])
                    widget_sizer(a->widget.frame.children[p]);
            }
            /* temp1 is for comparing and storing the largest value */
            int temp1;
            /* temp2 is for setting the size of the frame itself */
            /* temp3 is for keeping track of the number of non-zero sized elements */
            int temp2, temp3 = 0;

            sFrame *af = &a->widget.frame;
            /* WIDTH */
            for(int i = 0; i < MAX_GRID_COLS; i++) {
                temp1 = 0;
                for(int j = 0; j < MAX_GRID_ROWS; j++) {
                    /* Finding the largest width of each element in the column */
                    if(af->grid[i][j])
                        temp1 = af->grid[i][j]->size.x > temp1 ? af->grid[i][j]->size.x : temp1;
                }
                af->cols_size[i] = temp1;
                if (temp1 > 0)
                    ++temp3;
                temp2 += temp1;
            }
            /* 
             * Formula is 2 for the side spaces + 
             * temp3 (number of non-zero sized elements) - 1 for the inbetween spaces between widgets +
             * temp2 (cummalative addition of widest widgets)
             */
            if (a != w_root)
                a->size.x = 2 + (temp3 - 1) + temp2;
            /* Resetting temps for height, temp1 gets reset in the loop so its redundant doing it here */
            temp2 = 0;
            temp3 = 0;


            
            /* HEIGHT */
            for(int k = 0; k < MAX_GRID_ROWS; k++) {
                temp1 = 0;
                for(int l = 0; l < MAX_GRID_COLS; l++) {
                    /* Finding the largest height of each element in the column */
                    if(af->grid[l][k])
                        temp1 = af->grid[l][k]->size.y > temp1 ? af->grid[l][k]->size.y : temp1;
                }
                af->rows_size[k] = temp1;
                if (temp1 > 0)
                    ++temp3;
                temp2 += temp1;
            }
            /* refer to above for formula */
            if (a != w_root)
                a->size.y = 2 + (temp3 - 1) + temp2;

            /* Filling out the empty slots of cols_size and rows_size */
            for (int t = 0; t < MAX_GRID_ROWS; t++) {
                if (!af->rows_size[t])
                    af->rows_size[t] = 0;
            }

            for (int f = 0; f < MAX_GRID_ROWS; f++) {
                if (!af->cols_size[f])
                    af->cols_size[f] = 0;
            }
            break;
        case BUTTON:
            a->size.x = wcslen(a->widget.button.text);
            a->size.y = 1;
            break;
        default:
            break;
    }
}

void widget_positioner(sWidget *a) {
    /*
     * Sets up realsize and pos structs in the widget recursively. Generally the top level widget of
     * the window should be given as the argument.
     * Called when the program starts and when windows are created or resized.
     * This relies on previous calls to set the cursor into position;
     */

    /* Struct for the position of the "cursor" that places everything down */
    static sSize s_cursor = {0, 0};
    
    switch (a->type) {
        case FRAME:
            a->pos = s_cursor;
            sSize s_topleft = s_cursor;
            //s_cursor = add_sSize(s_cursor, (sSize) {1, 1});
            sFrame *af = &a->widget.frame;
            for (int i = 0; i < MAX_GRID_COLS; i++) {
                for (int j = 0; j < MAX_GRID_ROWS; j++) {
                    if (af->grid[i][j]) {
                        widget_positioner(af->grid[i][j]);
                        s_cursor.y += af->rows_size[j] + 1;
                    }
                }
                s_cursor.x += af->cols_size[i] + 1;
                s_cursor.y = s_topleft.y;
            }
            s_cursor = s_topleft;
            break;
        case BUTTON:
            a->pos = s_cursor;
            break;
        default:
            break;
    }
}

sSize add_sSize(sSize a, sSize b) {
    sSize s_return;
    s_return.x = a.x + b.x;
    s_return.y = a.y + b.y;
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
