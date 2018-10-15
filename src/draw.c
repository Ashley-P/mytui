#include <stddef.h>
#include "draw.h"
#include "tui.h"
#include "utils.h"
#include "input.h"

#define SHOW_MARGIN 0 /* Debugging stuff because the margin isn't usually drawn */
#define DRAW_MARGIN draw_box(a->pos.x, a->pos.y, a->rsize.x, a->rsize.y, 0, 0x00);

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

void reset_buf(CHAR_INFO *arr, const size_t len) {
    for (int i = 0; i < len; i++) {
        (arr + i)->Char.UnicodeChar = L' ';
        (arr + i)->Attributes = 0x00;
    }
}

void draw_line(const int x, const int y, const int len, const int direction, const unsigned char colour) {
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
void draw_box(const int x, const int y, const int width, const int height, const int fill, unsigned char colour) {
    if (fill) {
        for (int i = 0; i < width; i++) {
            draw_line(x + i, y, height, VERTICAL, colour);
        }
    } else {
        draw_line(x            , y             , width,  HORIZONTAL, colour); // Top Border
        draw_line(x            , y + height - 1, width,  HORIZONTAL, colour); // Bottom Border
        draw_line(x            , y             , height, VERTICAL,   colour); // Left Border
        draw_line(x + width - 1, y             , height, VERTICAL,   colour); // Right Border
    }
}

void draw_str(const wchar_t *str, const size_t len, const int x, const int y) {
    for(int i = 0; i < len; i++) {
        (*tui_current_screen + (x + i) + (y * sn_screenwidth))->Char.UnicodeChar = *(str + i);
    }
}

void draw_border_padding_content(const sWidget *a, unsigned char border_colour, unsigned char padding_colour,
        unsigned char content_colour) {
    /* Margin showing for debug since it's usually not coloured */
#if SHOW_MARGIN
    DRAW_MARGIN
#endif

    /* Border */
    if (a->bsize.x > 0 || a->bsize.y > 0) {
        draw_box(a->pos.x + a->msize.x, a->pos.y + a->msize.y,
                 a->rsize.x - (a->msize.x * 2), a->rsize.y - (a->msize.y * 2), 1, border_colour);
    }

    /* Disabled widget check */
    if (a->state == DISABLED || check_disable(a)) {
        draw_box(a->pos.x + a->msize.x + a->bsize.x, a->pos.y + a->msize.y + a->bsize.y,
                 a->rsize.x - (a->msize.x * 2) - (a->bsize.x * 2),
                 a->rsize.y - (a->msize.y * 2) - (a->bsize.y * 2), 1, 0x80);
    return;
    }

    /* Padding */
    if (a->psize.x > 0 || a->psize.y > 0) {
        draw_box(a->cpos.x - a->psize.x, a->cpos.y - a->psize.y,
                 a->csize.x + (a->psize.x * 2),
                 a->csize.y + (a->psize.y * 2), 1, padding_colour);
    }

    /* Content */
    draw_box(a->cpos.x, a->cpos.y, a->csize.x, a->csize.y, 1, content_colour);
}

void draw_frame(const sWidget *a, const int fill) {
    draw_border_padding_content(a, a->bcolour, a->pcolour, a->ccolour);

    /* Drawing the text on the border */
    /* Return if no text or if no top border*/
    if (wcscmp(L"", a->widget.frame.label.text) == 0 || a->bsize.y == 0) return;

    wchar_t *text = a->widget.frame.label.text;
    size_t len = a->widget.frame.label.len;
    int x = a->widget.frame.label.anchor;

    switch (x) {
        case NORTH:
            draw_str(text, len, a->pos.x + ((int) (a->rsize.x / 2)) - ((int) (len / 2)),
                                a->pos.y + a->msize.y + ((int) (a->bsize.y / 2)));
            break;
        case SOUTH:
            draw_str(text, len, a->pos.x + ((int) (a->rsize.x / 2)),
                                a->pos.y + a->rsize.y - a->msize.y - ((int) (a->bsize.y / 2)));
            break;
        case NORTH | WEST:
            draw_str(text, len, a->pos.x + a->msize.x + a->bsize.x,
                                a->pos.y + a->msize.y + ((int) (a->bsize.y / 2)));
            break;
        case NORTH | EAST:
            draw_str(text, len, a->pos.x + a->rsize.x - (a->msize.x + a->bsize.x + len),
                                a->pos.y + a->msize.y + ((int) (a->bsize.y / 2)));
            break;
        case SOUTH | WEST:
            draw_str(text, len, a->pos.x + a->msize.x + a->bsize.x,
                                a->pos.y + a->rsize.y - a->msize.y - ((int) (a->bsize.y / 2)));
            break;
        case SOUTH | EAST:
            draw_str(text, len, a->pos.x + a->rsize.x - (a->msize.x + a->bsize.x + len),
                                a->pos.y + a->rsize.y - a->msize.y - ((int) (a->bsize.y / 2)));
            break;
        default:
            /* Skip drawing for other anchors */
            return;
    }
}

void draw_button(const sWidget *a) {
    draw_border_padding_content(a, a->bcolour, a->pcolour, a->ccolour);

    /* Drawing the label */
    wchar_t *text = a->widget.button.label.text;
    size_t len = a->widget.button.label.len;
    int x = a->widget.button.label.anchor;

    switch (x) {
        case NORTH:
            draw_str(text, len, a->pos.x + ((int) (a->rsize.x / 2)) - ((int) (len / 2)),
                                a->cpos.y);
            break;
        case SOUTH:
            draw_str(text, len, a->pos.x + ((int) (a->rsize.x / 2)) - ((int) (len / 2)),
                                a->cpos.y + a->csize.y - 1);
            break;
        case EAST:
            draw_str(text, len, a->cpos.x + (a->csize.x - 1) - len,
                                a->cpos.y + ((int) (a->csize.y / 2)));
            break;
        case WEST:
            draw_str(text, len, a->cpos.x,
                                a->cpos.y + ((int) (a->csize.y / 2)));
            break;
        case NORTH | WEST:
            draw_str(text, len, a->cpos.x,
                                a->cpos.y);
            break;
        case NORTH | EAST:
            draw_str(text, len, a->cpos.x + (a->csize.x - 1) - len,
                                a->cpos.y);
            break;
        case SOUTH | WEST:
            draw_str(text, len, a->cpos.x,
                                a->cpos.y + a->csize.y - 1);
            break;
        case SOUTH | EAST:
            draw_str(text, len, a->cpos.x + (a->csize.x - 1) - len,
                                a->cpos.y + a->csize.y - 1);
            break;
        default:
            draw_str(text, len, a->pos.x + ((int) (a->rsize.x / 2)) - ((int) (len / 2)),
                                a->cpos.y + ((int) (a->csize.y / 2)));
            break;
    }

    switch (a->state) {
        case HOVER:
            draw_border_padding_content(a, a->bcolour, a->pcolour, 0x50);
            break;
        case PRESS:
            draw_border_padding_content(a, a->bcolour, a->pcolour, 0x60);
            break;
        default:
            break;

    }
}

void draw_label(const sWidget *a) {
    draw_border_padding_content(a, a->bcolour, a->pcolour, a->ccolour);
    draw_str(a->widget.label.text, a->widget.label.len, a->cpos.x, a->cpos.y);
}

void draw_checkbox(const sWidget *a) {
    draw_border_padding_content(a, a->bcolour, a->pcolour, a->ccolour);
    switch (a->widget.cbox.label.anchor) {
        case E:
            if (a->state == HOVER || a->state == PRESS)
                draw_box(a->cpos.x, a->cpos.y, 1, 1, 1, 0x80);
            else
                draw_box(a->cpos.x, a->cpos.y, 1, 1, 1, 0xF0);

            /* drawing checkbox */
            if (a->widget.cbox.active == 1)
                draw_str(L"\u25A0", 1, a->cpos.x, a->cpos.y);
            else if (a->widget.cbox.active == 2)
                draw_str(L"~", 1, a->cpos.x, a->cpos.y);
            draw_str(a->widget.cbox.label.text, a->widget.cbox.label.len, (a->cpos.x + 2), a->cpos.y);
            break;
        case W:
            if (a->state == HOVER || a->state == PRESS)
                draw_box((a->cpos.x + a->widget.cbox.label.len + 1), a->cpos.y, 1, 1, 1, 0x80);
            else
                draw_box((a->cpos.x + a->widget.cbox.label.len + 1), a->cpos.y, 1, 1, 1, 0xF0);

            /* drawing checkbox */
            if (a->widget.cbox.active == 1)
                draw_str(L"\u25A0", 1, (a->cpos.x + a->widget.cbox.label.len + 1), a->cpos.y);
            else if (a->widget.cbox.active == 2)
                draw_str(L"~", 1, (a->cpos.x + a->widget.cbox.label.len + 1), a->cpos.y);
            draw_str(a->widget.cbox.label.text, a->widget.cbox.label.len, a->cpos.x, a->cpos.y);
            break;
        default:
            tui_err(TUI_ERROR,
                    0,
                    "Error in function draw_checkbox: a->widget.checkbox.label.anchor is incorrect");
            break;
    }
}

void draw_radiobutton(const sWidget *a) {
    draw_border_padding_content(a, a->bcolour, a->pcolour, a->ccolour);
    switch (a->widget.rbutton.label.anchor) {
        case E:
            /* Drawing the label */
            draw_str(a->widget.rbutton.label.text, a->widget.rbutton.label.len, (a->cpos.x + 2), a->cpos.y);
            /* Drawing the dot */
            if (a->widget.rbutton.active == 0)
                draw_str(L"\u25CB", 1, a->cpos.x, a->cpos.y);
            else if (a->widget.rbutton.active == 1)
                draw_str(L"\u25CF", 1, a->cpos.x, a->cpos.y);
            break;
        case W:
            /* Drawing the label */
            draw_str(a->widget.rbutton.label.text, a->widget.rbutton.label.len, a->cpos.x, a->cpos.y);
            /* Drawing the dot */
            if (a->widget.rbutton.active == 0)
                draw_str(L"\u25CB", 1, (a->cpos.x + a->widget.rbutton.label.len + 1), a->cpos.y);
            else if (a->widget.rbutton.active == 1)
                draw_str(L"\u25CF", 1, (a->cpos.x + a->widget.rbutton.label.len + 1), a->cpos.y);
            break;
        default:
            tui_err(TUI_ERROR,
                    0,
                    "Error in function draw_radiobutton: a->widget.radiobutton.label.anchor is incorrect");
            break;
    }
}

void draw_canvas(const sWidget *a) {
    draw_border_padding_content(a, a->bcolour, a->pcolour, a->ccolour);
    /* Directly copying the CHAR_INFO structs into the screen */
    unsigned short width  = a->widget.canvas.width;
    unsigned short height = a->widget.canvas.height;
    CHAR_INFO *c = a->widget.canvas.canvas;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            *(*tui_current_screen + a->cpos.x + i + ((a->cpos.y + j) * sn_screenwidth)) = *(c + i + (j * height));
        }
    }
}

void draw_field(sWidget *a) {
    /* TODO: Blinking Cursor */
    draw_border_padding_content(a, a->bcolour, a->pcolour, a->ccolour);

    /* Drawing text and making sure it doesn't leave the boundaries of the field */
    if (a->widget.field.text.len >= a->csize.x) {
        wchar_t temp[a->csize.x];
        for (int i = 0; i < a->csize.x; i++) {
            temp[a->csize.x - 1 - i] = *(a->widget.field.text.text + a->widget.field.text.len - i);
        }
        draw_str(temp, a->csize.x, a->cpos.x, a->cpos.y);
    }
    else
        draw_str(a->widget.field.text.text, a->widget.field.text.len, a->cpos.x, a->cpos.y);

    /* Drawing the cursor and making sure it doesn't leave the field */
    if (a == focused_wid) {
        if (a->widget.field.cursor.x >= a->csize.x) {
            if (a->widget.field.cursor_active || a->widget.field.cursor_force_on)
                draw_box(a->cpos.x + a->csize.x - 1, a->cpos.y + a->widget.field.cursor.y, 1, 1, 1, 0x00);
        }
        else {
            if (a->widget.field.cursor_active || a->widget.field.cursor_force_on)
                draw_box(a->cpos.x + a->widget.field.cursor.x, 
                         a->cpos.y + a->widget.field.cursor.y, 1, 1, 1, 0x00);
        }

        /* Blinking Cursor */
        if (a->widget.field.cursor_blink + (0.5 * CLOCKS_PER_SEC) < clock()) {
            a->widget.field.cursor_blink = clock();
            a->widget.field.cursor_active = !a->widget.field.cursor_active;
            a->widget.field.cursor_force_on = 0;
        }
    }
}
