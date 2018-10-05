#include <stdbool.h>
#include <stddef.h>
#include "draw.h"
#include "tui.h"
#include "utils.h"

#define H HORIZONTAL
#define V VERTICAL
#define HORIZONTAL 1
#define VERTICAL   2

/* 
 * Unicode chars
 * \u25A0 == Black Square
 * \u25CB == White Circle
 * \u25CF == Black Circle
 */

void reset_buf() {
    for(int i = 0; i < i_bufsize; i++) {
        (*tui_current_screen + i)->Char.UnicodeChar = L' ';
        (*tui_current_screen + i)->Attributes = 0x70;
    }
}

void draw_line(const int x, const int y, const int len, const int direction, const int colour) {
    /* Function draws from the coords provided towards the right or downwards */
    if (direction != HORIZONTAL && direction != VERTICAL) {
        tui_err(TUI_ERROR, 0, 
                "Error in draw_line: Incorrect argument for direction; Expected 1 or 2 Got %d", direction);
        return;
    }
    /* Throw error if the line is too long for the screen */
    if (direction == HORIZONTAL && x + len > sn_screenwidth) {
        tui_err(TUI_ERROR, 0, "Error in draw_line: Length too long");
        return;
    }
    if (direction == VERTICAL && y + len > sn_screenwidth) {
        tui_err(TUI_ERROR, 0, "Error in draw_line: Length too long");
        return;
    }
    
    if (direction == HORIZONTAL) {
        for (int i = 0; i < len; i++) {
            (*tui_current_screen + (x + i) + (y * sn_screenwidth))->Attributes = colour;
        }
    }

    if (direction == VERTICAL) {
        for (int i = 0; i < len; i++) {
            (*tui_current_screen + x + ((y + i) * sn_screenwidth))->Attributes = colour;
        }
    }

}
void draw_box(int x, int y, const int width, const int height, const bool fill, int colour) {
    if (fill) {
        for (int i = 0; i < width; i++) {
            draw_line(x + i, y, height - 1, VERTICAL, colour);
        }
    } else {
        draw_line(x            , y             , width,  HORIZONTAL, colour); // Top Border
        draw_line(x            , y + height - 1, width,  HORIZONTAL, colour); // Bottom Border
        draw_line(x            , y             , height, VERTICAL,   colour); // Left Border
        draw_line(x + width - 1, y             , height, VERTICAL,   colour); // Right Border
    }
}

void draw_str(const wchar_t *str, const size_t len, int x, int y) {
    for(int i = 0; i < len; i++) {
        (*tui_current_screen + (x + i) + (y * sn_screenwidth))->Char.UnicodeChar = *(str + i);
    }
}

void draw_frame(sWidget *a, const bool fill) {
    /* Drawing the frame border */
    switch (a->state) {
        case DISABLED:
            draw_box(a->pos.x, a->pos.y, a->rsize.x, a->rsize.y, 1, 0x80);
            break;
        default:
            draw_box(a->pos.x, a->pos.y, a->rsize.x, a->rsize.y, fill, 0x90);
            break;
    }
    
    /* Drawing the text */
    /* Return if no text */
    if (!wcscmp(L"", a->widget.frame.label.text)) return;

    int strx; int stry;
    int x = a->widget.frame.label.anchor;
    switch (x) {
        case N:
            strx = a->pos.x + ((int) (a->csize.x / 2)) - ((int) (a->widget.frame.label.len / 2));
            stry = a->pos.y;
            break;
        case S:
            strx = a->pos.x + ((int) (a->csize.x / 2)) - ((int) (a->widget.frame.label.len / 2));
            stry = a->pos.y + a->csize.y - 1;
            break;
        case N | E:
            strx = a->pos.x + a->csize.x - a->widget.frame.label.len - 1;
            stry = a->pos.y;
            break;
        case N | W:
            strx = a->pos.x + 1;
            stry = a->pos.y;
            break;
        case S | E:
            strx = a->pos.x + a->csize.x - a->widget.frame.label.len - 1;
            stry = a->pos.y + a->csize.y - 1;
            break;
        case S | W:
            strx = a->pos.x + 1;
            stry = a->pos.y + a->csize.y - 1;
            break;
        case E: case W: case N|S|E: case N|S|W: case S|E|W: case N|E|W: case N|S|E|W: default:
            /* Just skip drawing the text */
            return;
    }
    draw_str(a->widget.frame.label.text, a->widget.frame.label.len, strx, stry);
}


void draw_button(const sWidget *a) {
    /* Offsetting for buttons that are larger than their text size */
    int x = a->cpos.x + ((int) (a->csize.x / 2)) - ((int) (a->widget.button.label.len / 2));
    int y = a->cpos.y + ((int) (a->csize.y / 2));

    /* TODO: Update to work with diagonals e.g NE, SW) */
    switch (a->widget.button.label.anchor) {
        case NORTH:
            y = a->cpos.y;
            break;
        case SOUTH:
            y = a->cpos.y + a->csize.y - 1;
            break;
        case EAST:
            x = a->cpos.x + a->csize.x - a->widget.button.label.len;
            break;
        case WEST:
            x = a->cpos.x;
            break;
        default:
            break;
    }

    draw_str(a->widget.button.label.text, a->widget.button.label.len, a->cpos.x, a->cpos.y);

    /* Traversing up the tree to see if any parents have the DISABLED state */
    sWidget *p = a->parent;
    int isDisabled = 0;
    while (p != NULL) {
        if (p->state == DISABLED) {isDisabled = 1; return;}
        else {p = p->parent;}
    }
    if (isDisabled) {
        draw_line(a->cpos.x, a->cpos.y, a->csize.x, HORIZONTAL, 0x80);
        return;
    }

    switch (a->state) {
        case NONE:
            draw_line(a->cpos.x, a->cpos.y, a->csize.x, HORIZONTAL, 0x40);
            break;
        case HOVER:
            draw_line(a->cpos.x, a->cpos.y, a->csize.x, HORIZONTAL, 0x50);
            break;
        case PRESS:
            draw_line(a->cpos.x, a->cpos.y, a->csize.x, HORIZONTAL, 0x60);
            break;
        default:
            break;
    }
}

void draw_label(const sWidget *a) {
    draw_str(a->widget.label.text, a->widget.label.len, a->pos.x, a->pos.y);
}

void draw_checkbox(const sWidget *a) {
    switch (a->widget.cbox.label.anchor) {
        case E:
            if (a->state == HOVER || a->state == PRESS)
                draw_box(a->pos.x, a->pos.y, 1, 1, 1, 0x80);
            else
                draw_box(a->pos.x, a->pos.y, 1, 1, 1, 0xF0);

            /* drawing checkbox */
            if (a->widget.cbox.active == 1)
                draw_str(L"\u25A0", 1, a->pos.x, a->pos.y);
            else if (a->widget.cbox.active == 2)
                draw_str(L"~", 1, a->pos.x, a->pos.y);
            draw_str(a->widget.cbox.label.text, a->widget.cbox.label.len, (a->pos.x + 2), a->pos.y);
            break;
        case W:
            if (a->state == HOVER || a->state == PRESS)
                draw_box((a->pos.x + a->widget.cbox.label.len + 1), a->pos.y, 1, 1, 1, 0x80);
            else
                draw_box((a->pos.x + a->widget.cbox.label.len + 1), a->pos.y, 1, 1, 1, 0xF0);

            /* drawing checkbox */
            if (a->widget.cbox.active == 1)
                draw_str(L"\u25A0", 1, (a->pos.x + a->widget.cbox.label.len + 1), a->pos.y);
            else if (a->widget.cbox.active == 2)
                draw_str(L"~", 1, (a->pos.x + a->widget.cbox.label.len + 1), a->pos.y);
            draw_str(a->widget.cbox.label.text, a->widget.cbox.label.len, a->pos.x, a->pos.y);
            break;
        default:
            tui_err(TUI_ERROR,
                    0,
                    "Error in function draw_checkbox: a->widget.checkbox.label.anchor is incorrect");
            break;
    }

    if (a->state == DISABLED)
        draw_box(a->pos.x, a->pos.y, a->csize.x, a->csize.y, 1, 0x80);
}

void draw_radiobutton(const sWidget *a) {
    switch (a->widget.rbutton.label.anchor) {
        case E:
            /* Drawing the label */
            draw_str(a->widget.rbutton.label.text, a->widget.rbutton.label.len, (a->pos.x + 2), a->pos.y);
            /* Drawing the dot */
            if (a->widget.rbutton.active == 0)
                draw_str(L"\u25CB", 1, a->pos.x, a->pos.y);
            else if (a->widget.rbutton.active == 1)
                draw_str(L"\u25CF", 1, a->pos.x, a->pos.y);
            break;
        case W:
            /* Drawing the label */
            draw_str(a->widget.rbutton.label.text, a->widget.rbutton.label.len, a->pos.x, a->pos.y);
            /* Drawing the dot */
            if (a->widget.rbutton.active == 0)
                draw_str(L"\u25CB", 1, (a->pos.x + a->widget.rbutton.label.len + 1), a->pos.y);
            else if (a->widget.rbutton.active == 1)
                draw_str(L"\u25CF", 1, (a->pos.x + a->widget.rbutton.label.len + 1), a->pos.y);
            break;
        default:
            tui_err(TUI_ERROR,
                    0,
                    "Error in function draw_radiobutton: a->widget.radiobutton.label.anchor is incorrect");
            break;
    }

    if (a->state == DISABLED)
        draw_box(a->pos.x, a->pos.y, a->csize.x, a->csize.y, 1, 0x80);
}
