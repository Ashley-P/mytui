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
void draw_box(int x, int y, const int width, const int height, const int fill, int colour) {
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

void draw_str(const wchar_t *str, const size_t len, int x, int y) {
    for(int i = 0; i < len; i++) {
        (*tui_current_screen + (x + i) + (y * sn_screenwidth))->Char.UnicodeChar = *(str + i);
    }
}

void draw_border_padding_content(sWidget *a, int border_colour, int padding_colour, int content_colour) {
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

void draw_frame(sWidget *a, const int fill) {
    draw_border_padding_content(a, 0x90, 0x70, 0x70);

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

void draw_button(sWidget *a) {
    draw_border_padding_content(a, 0x90, 0x70, 0x40);

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
            draw_border_padding_content(a, 0x90, 0x70, 0x50);
            break;
        case PRESS:
            draw_border_padding_content(a, 0x90, 0x70, 0x60);
            break;
        default:
            break;

    }
}

void draw_label(sWidget *a) {
    draw_border_padding_content(a, 0x90, 0x70, 0x70);
    draw_str(a->widget.label.text, a->widget.label.len, a->cpos.x, a->cpos.y);
}

void draw_checkbox(sWidget *a) {
    draw_border_padding_content(a, 0x90, 0x70, 0x70);
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

void draw_radiobutton(sWidget *a) {
    draw_border_padding_content(a, 0x90, 0x70, 0x70);
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
