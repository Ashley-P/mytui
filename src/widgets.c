#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include "const.h"
#include "widgets.h"
#include "draw.h"
#include "utils.h"



void assign_to_parent(sWidget *child, sWidget *parent) {
    /* 
     * Assigns children to parents
     * returns NULL if parent is NULL
     */
    if (!parent) {
        child->parent = NULL;
        return;
    }

    child->parent = parent;
    switch (parent->type) {
        case FRAME:
            parent->widget.frame.children[parent->widget.frame.numch] = child;
            parent->widget.frame.numch += 1;
            break;
        case BUTTON:
            tui_err(TUI_ERROR, 1, "Error in assign_to_parent. Wrong type for parent: type is BUTTON");
            break;
        case LABEL:
            tui_err(TUI_ERROR, 1, "Error in assign_to_parent. Wrong type for parent: type is LABEL");
            break;
        case CHECKBOX:
            tui_err(TUI_ERROR, 1, "Error in assign_to_parent. Wrong type for parent: type is CHECKBOX");
            break;
        default:
            break;
    }


}

sWidget * init_sWidget(sWidget *parent) {
    sWidget *ptr = (sWidget *)calloc(1, sizeof(sWidget));
    ptr->state   = NONE;
    ptr->rowspan = 0;
    ptr->colspan = 0;
    ptr->anchor  = C;
    assign_to_parent(ptr, parent);

    return ptr;
}

sWidget * tui_frame(sWidget *parent, wchar_t *text) {
    // sWidget setup
    sWidget *ptr = init_sWidget(parent);
    ptr->type    = FRAME;

    // sFrame setup
    ptr->widget.frame.numch        = 0;
    ptr->widget.frame.label.text   = text;
    ptr->widget.frame.label.len    = wcslen(text);
    ptr->widget.frame.label.anchor = N | W;

    /* Making sure each element is set to null in both arrays */
    for(int i = 0; i < MAX_CHILDREN; i++)
        ptr->widget.frame.children[i] = NULL;

    for(int j = 0; j < MAX_GRID_COLS; j++) {
        for(int k = 0; k < MAX_GRID_ROWS; k++) {
            ptr->widget.frame.grid[j][k] = NULL;
        }
    }

    return ptr;
}

void tui_root_frame() {
    w_root = tui_frame(NULL, L"");
    w_root->size.x = sn_screenwidth;
    w_root->size.y = sn_screenheight;
}

sWidget * tui_button(sWidget *parent, wchar_t *text, void(*callback)()) {
    /* sWidget setup */
    sWidget *ptr = init_sWidget(parent);
    ptr->type    = BUTTON;

    // sButton setup
    ptr->widget.button.label.text   = text;
    ptr->widget.button.label.len    = wcslen(text);
    ptr->widget.button.label.anchor = C;
    ptr->widget.button.callback     = callback;

    return ptr;
}

sWidget * tui_label(sWidget *parent, wchar_t *text) {
    /* sWidget setup */
    sWidget *ptr = init_sWidget(parent);
    ptr->type    = LABEL;

    ptr->widget.label.text   = text;
    ptr->widget.label.len    = wcslen(text);
    ptr->widget.label.anchor = C;

    return ptr;
}

sWidget * tui_checkbox(sWidget *parent, wchar_t *text) {
    sWidget *ptr = init_sWidget(parent);
    ptr->type    = CHECKBOX;
    
    ptr->widget.cbox.label.text   = text;
    ptr->widget.cbox.label.len    = wcslen(text);
    ptr->widget.cbox.label.anchor = W;
    ptr->widget.cbox.active       = 0;
    ptr->widget.cbox.len          = 0;

    for(int i = 0; i < MAX_CHILDREN; i++)
        ptr->widget.cbox.children[i] = NULL;

    return ptr;
}

sWidget * tui_radiobutton(sWidget *parent, wchar_t *text) {
    sWidget *ptr = init_sWidget(parent);
    ptr->type    = RADIOBUTTON;

    ptr->widget.rbutton.label.text   = text;
    ptr->widget.rbutton.label.len    = wcslen(text);
    ptr->widget.rbutton.label.anchor = W;
    ptr->widget.rbutton.active       = 0;

    return ptr;
}

sRadiobuttonLink * tui_radiobutton_link() {
    sRadiobuttonLink *ptr = (sRadiobuttonLink *)calloc(1, sizeof(sRadiobuttonLink));
    
    ptr->len    = 0;
    ptr->active = 0;
    ptr->old    = NULL;
    for(int i = 0; i < MAX_CHILDREN; i++) ptr->children[i] = NULL;

    return ptr;
}

void widget_sizer(sWidget *a) {
    /* Margins, paddings, borders and other things that can change size are computed here */
    /* TODO: Refactor this, I could probably redo this without a trillion loops */
    switch (a->type) {
        case FRAME:
            /* Tons of loops because I can't program */
            for(int p = 0; p < MAX_CHILDREN; p++) {
                if(a->widget.frame.children[p])
                    widget_sizer(a->widget.frame.children[p]);
            }
            /*
             * temp1 is for comparing and storing the largest value
             * temp2 is for setting the size of the frame itself
             * temp3 is for keeping track of the number of non-zero sized elements
             */
            int temp1;
            int temp2 = 0;
            int temp3 = 0;

            sFrame *af = &a->widget.frame;
            /* WIDTH */
            for(int i = 0; i < MAX_GRID_COLS; i++) {
                temp1 = 0;
                for(int j = 0; j < MAX_GRID_ROWS; j++) {
                    /* Finding the largest width of each element in the column */
                    if(af->grid[i][j]) {
                        //if(af->grid[i][j]->colspan < 2)
                        temp1 = af->grid[i][j]->size.x > temp1 ? af->grid[i][j]->size.x : temp1;
                    }
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
                    if(af->grid[l][k]) {
                        //if(af->grid[i][j]->colspan < 2)
                        temp1 = af->grid[l][k]->size.y > temp1 ? af->grid[l][k]->size.y : temp1;
                    }
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
            a->size.x = a->widget.button.label.len;
            a->size.y = 1;
            break;
        case LABEL:
            a->size.x = a->widget.label.len;
            a->size.y = 1;
            break;
        case CHECKBOX:
            a->size.x = a->widget.cbox.label.len + 2;
            a->size.y = 1;
        case RADIOBUTTON:
            a->size.x = a->widget.rbutton.label.len + 2;
            a->size.y = 1;
        default:
            break;
    }
}

void widget_span_sizer(sWidget *a) {
    switch(a->type) {
        case FRAME:
            ;
            sFrame *af = &a->widget.frame;
            /* Calling the sizer for children */
            for(int p = 0; p < MAX_CHILDREN; p++) {
                if(af->children[p])
                    widget_sizer(af->children[p]);
            }

            /*
             * Setting the widget to the right size.
             * First we iterate through the grid to find the
             * widgets with spans > 2.
             * Then check if the grid positions it wants to span to exist.
             * Then is sets its size using cols_size/rows_size of the spanned
             * grid sections.
             */
            /* TODO: Refactor */
            /* Could reduce the two top level loops to one using some maths */
            for(int i = 0; i < MAX_GRID_COLS; i++) {
                for(int j = 0; j < MAX_GRID_ROWS; j++) {
                    if (af->grid[i][j]) {

                        int k;
                        sWidget *wid = af->grid[i][j];

                        /* Row span */
                        if(wid->rowspan >= 2) {
                            wid->size.y = 0;
                            for(k = 0; k < wid->rowspan; k++) {
                                /*
                                if(af->grid[i][j + k]) // Checking the next grid spaces
                                    break;
                                */
                                wid->size.y += af->rows_size[j + k];
                            }
                            wid->size.y += (wid->rowspan - 1);
                        }
                        /* Col span */
                        if(wid->colspan >= 2) {
                            wid->size.x = 0;
                            for(k = 0; k < wid->colspan; k++) {
                                /*
                                if(af->grid[i + k][j]) // Checking the next grid spaces
                                    break;
                                */
                                wid->size.x += af->cols_size[i + k];
                            }
                            wid->size.x += (wid->colspan - 1);
                        }
                    }
                }
            }
            break;
        default:
            break;
    }
}

void widget_anchorer(sWidget *a, int *pcols, int *prows) {
    enum eAnchor anchor = a->anchor;
    /* Really Questionable usage of goto */
    /* TODO: do a real fix for spanning */
    if (a->rowspan) goto l1;
    if (a->colspan) goto l2;

    /* Centres the widget */
    l1:
    a->pos.x = a->pos.x + ((int) pcols[a->gridpos.x] / 2) - ((int) a->size.x / 2);
    if (a->rowspan) goto l5;
    l2:
    a->pos.y = a->pos.y + ((int) prows[a->gridpos.y] / 2) - ((int) a->size.y / 2);

    /* Widget gets moved back up or left if either of these conditionals are true */
    l5:
    if ((N | S) == (anchor & (N | S)) && !a->rowspan) {
        a->pos.y = a->pos.y - ((int) prows[a->gridpos.y] / 2) + ((int) a->size.y / 2);
        a->size.y = prows[a->gridpos.y];
    }
    if ((E | W) == (anchor & (E | W)) && !a->colspan) {
        a->pos.x = a->pos.x - ((int) pcols[a->gridpos.x] / 2) + ((int) a->size.x / 2);
        a->size.x = pcols[a->gridpos.x];
    }
    if ((N | S | E | W) == (anchor & (N | S | E | W))) return;

    if ((N | S) == (anchor & (N | S)) && !a->rowspan) goto l4;
    if ((E | W) == (anchor & (E | W)) && !a->colspan) goto l3;

    l3:
    if (anchor & N) a->pos.y = a->pos.y - ((int) prows[a->gridpos.y] / 2);
    if (anchor & S) a->pos.y = a->pos.y + ((int) prows[a->gridpos.y] / 2);
    if (anchor ^ (E | W) && !a->colspan) goto l4;
    return;
    l4:
    if (anchor & E) a->pos.x = a->pos.x + ((int) pcols[a->gridpos.x] / 2) - ((int) a->size.x / 2);
    if (anchor & W) a->pos.x = a->pos.x - ((int) pcols[a->gridpos.x] / 2) + ((int) a->size.x / 2);
    return;
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
            sSize s_topleft2 = s_cursor;
            s_cursor = add_sSize(s_cursor, (sSize) {1, 1});
            sSize s_topleft1 = s_cursor;
            sFrame *af = &a->widget.frame;
            for (int i = 0; i < MAX_GRID_COLS; i++) {
                for (int j = 0; j < MAX_GRID_ROWS; j++) {
                    if (af->grid[i][j]) {
                        widget_positioner(af->grid[i][j]);
                    }
                    s_cursor.y += af->rows_size[j] + 1;
                    
                }
                s_cursor.x += af->cols_size[i] + 1;
                s_cursor.y = s_topleft1.y;
            }
            s_cursor = s_topleft2;
            break;
        case BUTTON: case LABEL: case CHECKBOX: case RADIOBUTTON:
            a->pos = s_cursor;
            break;
        default:
            break;
    }

    /* Calling anchorer here to prevent a bunch of recursion. Also skip w_root */
    if (a == w_root)
        return;

    switch (a->parent->type) {
        case FRAME:
            widget_anchorer(a, a->parent->widget.frame.cols_size, a->parent->widget.frame.rows_size);
            break;
        default:
            break;
    }
}

void redraw_widgets(sWidget *a) {
    widget_sizer(a);
    widget_span_sizer(a);
    widget_positioner(a);
}

sSize add_sSize(const sSize a, const sSize b) {
    sSize s_return;
    s_return.x = a.x + b.x;
    s_return.y = a.y + b.y;
    return s_return;
}

// Used to return a sSize struct with the largest of each member
sSize max_sSize(const sSize a, const sSize b) {
    sSize s_return;
    s_return.x = a.x > b.x ? a.x : b.x;
    s_return.y = a.y > b.y ? a.y : b.y;
    return s_return;
}

void checkbox_add(sWidget *a, sWidget *b) {
    if (a->type != CHECKBOX || a->type != CHECKBOX) {
        tui_err(TUI_WARNING, 0, "Error in function checkbox_add. Wrong widget type: Expected CHECKBOX");
        return;
    }

    if (a->widget.cbox.len == MAX_CHILDREN) {
        tui_err(TUI_WARNING, 0, "Error in function checkbox_add. Too many children");
    }

    a->widget.cbox.children[a->widget.cbox.len++] = b;
    b->widget.cbox.parent = a;
}

void radiobutton_link(sRadiobuttonLink *link, int count, ...) {
    va_list arg;
    va_start(arg, count);
    while(count != 0) {
        /* Getting the argument from the list */
        uintptr_t a = va_arg(arg, uintptr_t);
        sWidget *b = (sWidget *) a;

        if (b->type != RADIOBUTTON) return;
        if (link->len >= MAX_CHILDREN) {
            tui_err(TUI_WARNING, 0, "Error in function radiobutton_link: link has maximum children");
            return;
        }
        link->children[(link->len)++] = &b->widget.rbutton;
        b->widget.rbutton.parent = link;
        count--;
    }
    va_end(arg);
}

void grid_set(sWidget *widget, int col, int row) {
    /* This function sets the grid position in its parent */
    /* Make sure there is a warning if a grid position gets overwritten */
    /* Basic checking for arguments */

    if (col >= MAX_GRID_COLS || col < 0) {
       tui_err(TUI_ERROR,
               1,
               "Error in function grid_set. Error incorrect col value; Expected 0 < col < %d - Got %d",
               MAX_GRID_COLS,
               col);
    }
    if (row >= MAX_GRID_ROWS || row < 0) {
       tui_err(TUI_ERROR,
               1,
               "Error in function grid_set. Error incorrect row value; Expected 0 < row < %d - Got %d",
               MAX_GRID_ROWS,
               row);
    }

    /* Actually assigning the widget to the grid */
    /* Don't need to call parent_widget_type because it get called on the constructor for the widget */
    switch(widget->parent->type) {
        case FRAME:
            if (widget->parent->widget.frame.grid[col][row])
                tui_err(TUI_WARNING,
                        0,
                        "Error in function grid_set. Grid position is already taken, overwritten anyway");
            widget->parent->widget.frame.grid[col][row] = widget;
            widget->gridpos.x = col;
            widget->gridpos.y = row;
            break;
        default:
            break;

    }
}
