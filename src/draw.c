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

void draw_frame(int x, int y, const int width, const int height, const bool fill, int colour) {
    draw_box(x, y, width, height, fill, colour);
}


void draw_button(const sWidget *a) {
    /* TODO: Rewrite to work with anchoring */
    /* Offsetting for buttons that are larger than their text size */
    //int x = a->pos.x + ((int) (a->size.x / 2)) - ((int) (wcslen(a->widget.button.text) / 2));
    int x = a->pos.x + ((int) (a->size.x / 2)) - ((int) (a->widget.button.text.len / 2));
    int y = a->pos.y + ((int) (a->size.y / 2));

    switch (a->anchor) {
        case NORTH:
            y = a->pos.y;
            break;
        case SOUTH:
            y = a->pos.y + a->size.y - 1;
            break;
        case EAST:
            x = a->pos.x + a->size.x - a->widget.button.text.len;
            break;
        case WEST:
            x = a->pos.x;
            break;
        default:
            break;
    }

    draw_str(a->widget.button.text.text, a->widget.button.text.len, x, y);

    switch (a->state) {
        case NONE:
            draw_box(a->pos.x, a->pos.y, a->size.x, a->size.y, 1, 0x40);
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
