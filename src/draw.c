#include <stdbool.h>
#include <stddef.h>
#include "draw.h"
#include "tui.h"
#include "utils.h"


void reset_buf() {
    for(int i = 0; i < i_bufsize; i++) {
        (*tui_current_screen + i)->Char.UnicodeChar = L' ';
        (*tui_current_screen + i)->Attributes = 0x70;
    }
}

void draw_box(int x, int y, const int width, const int height, const bool fill, int colour) {
    /*
    // Shifting the coords by -1, -1 to make sense on the screen
    x = x - 1;
    y = y - 1;
    */

    // Checking if the box goes off the screen to prevent weird drawing issues
    if (x + width > sn_screenwidth || y + height > sn_screenheight) {
        tui_err(TUI_WARNING, 0, "draw_box: parameters too large");
        return;
    }

    // Filled box
    if (fill == true) {
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                (*tui_current_screen + (x + j) + ((y + i) * sn_screenwidth ))->Attributes = colour;
            }
        }
    }

    // Non-filled box
    if (fill == false) {
        for(int i = 0; i < height; i++) {
            // Left Border
            (*tui_current_screen + x + ((y + i) * sn_screenwidth))->Attributes = colour;
            // Right Border
            (*tui_current_screen + (x + width - 1) + ((y + i) * sn_screenwidth))->Attributes = colour;
        }

        for(int j = 0; j < width; j++) {
            // Top Border
            (*tui_current_screen + (x + j) + (y * sn_screenwidth))->Attributes = colour;
            // Bottom Border
            (*tui_current_screen + (x + j) + ((y + height - 1) * sn_screenwidth))->Attributes = colour;
        }
    }
}

void draw_str(const wchar_t *str, const size_t len, int x, int y) {
    //int str_len = wcslen(str);
    for(int i = 0; i < len; i++) {
        (*tui_current_screen + (x + i) + (y * sn_screenwidth))->Char.UnicodeChar = *(str + i);
    }
}

void draw_frame(sWidget *a, const bool fill) {
    /* Drawing the frame border */
    switch (a->state) {
        case DISABLED:
            draw_box(a->pos.x, a->pos.y, a->size.x, a->size.y, 1, 0x80);
            break;
        default:
            draw_box(a->pos.x, a->pos.y, a->size.x, a->size.y, fill, 0x90);
            break;
    }
    
    /* Drawing the text */
    /* Return if no text */
    if (!wcscmp(L"", a->widget.frame.label.text)) return;

    int strx; int stry;
    int x = a->widget.frame.label.anchor;
    switch (x) {
        case N:
            strx = a->pos.x + ((int) (a->size.x / 2)) - ((int) (a->widget.frame.label.len / 2));
            stry = a->pos.y;
            break;
        case S:
            strx = a->pos.x + ((int) (a->size.x / 2)) - ((int) (a->widget.frame.label.len / 2));
            stry = a->pos.y + a->size.y - 1;
            break;
        case N | E:
            strx = a->pos.x + a->size.x - a->widget.frame.label.len - 1;
            stry = a->pos.y;
            break;
        case N | W:
            strx = a->pos.x + 1;
            stry = a->pos.y;
            break;
        case S | E:
            strx = a->pos.x + a->size.x - a->widget.frame.label.len - 1;
            stry = a->pos.y + a->size.y - 1;
            break;
        case S | W:
            strx = a->pos.x + 1;
            stry = a->pos.y + a->size.y - 1;
            break;
        case E: case W: case N|S|E: case N|S|W: case S|E|W: case N|E|W: case N|S|E|W: default:
            /* Just skip drawing the text */
            return;
    }
    draw_str(a->widget.frame.label.text, a->widget.frame.label.len, strx, stry);
}


void draw_button(const sWidget *a) {
    /* Offsetting for buttons that are larger than their text size */
    int x = a->pos.x + ((int) (a->size.x / 2)) - ((int) (a->widget.button.label.len / 2));
    int y = a->pos.y + ((int) (a->size.y / 2));

    /* TODO: Update to work with diagonals e.g NE, SW) */
    switch (a->widget.button.label.anchor) {
        case NORTH:
            y = a->pos.y;
            break;
        case SOUTH:
            y = a->pos.y + a->size.y - 1;
            break;
        case EAST:
            x = a->pos.x + a->size.x - a->widget.button.label.len;
            break;
        case WEST:
            x = a->pos.x;
            break;
        default:
            break;
    }

    draw_str(a->widget.button.label.text, a->widget.button.label.len, x, y);

    /* Traversing up the tree to see if any parents have the DISABLED state */
    sWidget *p = a->parent;
    int isDisabled = 0;
    while (p != NULL) {
        if (p->state == DISABLED) {isDisabled = 1; return;}
        else {p = p->parent;}
    }
    if (isDisabled) {
        draw_box(a->pos.x, a->pos.y, a->size.x, a->size.y, 1, 0x80);
        return;
    }

    switch (a->state) {
        case NONE:
            draw_box(a->pos.x, a->pos.y, a->size.x, a->size.y, 1, 0x40);
            break;
        case DISABLED:
            draw_box(a->pos.x, a->pos.y, a->size.x, a->size.y, 1, 0x80);
            break;
        case HOVER:
            draw_box(a->pos.x, a->pos.y, a->size.x, a->size.y, 1, 0x50);
            break;
        case PRESS:
            draw_box(a->pos.x, a->pos.y, a->size.x, a->size.y, 1, 0x60);
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
            if (a->state == HOVER || a->state == PRESS) {
                draw_box(a->pos.x, a->pos.y, 1, 1, 1, 0x80);
            } else {
                draw_box(a->pos.x, a->pos.y, 1, 1, 1, 0xF0);
            }
            /* drawing checkbox */
            if (a->widget.cbox.active)
                draw_str(L"\u25A0", 1, a->pos.x, a->pos.y);
            draw_str(a->widget.cbox.label.text, a->widget.cbox.label.len, (a->pos.x + 2), a->pos.y);
            break;
        case W:
            if (a->state == HOVER || a->state == PRESS) {
                draw_box((a->pos.x + a->widget.cbox.label.len + 1), a->pos.y, 1, 1, 1, 0x80);
            } else {
                draw_box((a->pos.x + a->widget.cbox.label.len + 1), a->pos.y, 1, 1, 1, 0xF0);
            }
            /* drawing checkbox */
            if (a->widget.cbox.active)
                draw_str(L"\u25A0", 1, (a->pos.x + a->widget.cbox.label.len + 1), a->pos.y);
            draw_str(a->widget.cbox.label.text, a->widget.cbox.label.len, a->pos.x, a->pos.y);
            break;
        default:
            tui_err(TUI_ERROR, 0, "Error in draw_checkbox(): a->widget.checkbox.label.anchor is incorrect");
            break;
    }

    if (a->state == DISABLED)
        draw_box(a->pos.x, a->pos.y, a->size.x, a->size.y, 1, 0x80);
}
