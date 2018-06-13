#include <stdbool.h>
#include "draw.h"
#include "tui.h"
#include "utils.h"


void reset_buf(wchar_t *buf) {
    for(int i = 0; i < i_bufsize; i++)
        *(buf + i) = L' ';
    *(buf + i_bufsize) = L'\0';
}

void draw_box(wchar_t *buf, int x, int y, const int width, const int height, const bool fill) {
    // Shifting the coords by -1, -1 to make sense on the screen
    x = x - 1;
    y = y - 1;

    // Checking if the box goes off the screen to prevent weird drawing issues
    if (x + width > sn_screenwidth || y + width > sn_screenheight) {
        tui_err("draw_box: parameters too large", 0);
        return;
    }

    // Filled box
    if (fill == true) {
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                *(buf + (x + j) + ((y + i) * sn_screenwidth )) = L'#';
            }
        }
    }

    // Non-filled box
    if (fill == false) {
        for(int i = 0; i < height; i++) {
            *(buf + x + ((y + i) * sn_screenwidth)) = L'#';                     // Left Border
            *(buf + (x + width - 1) + ((y + i) * sn_screenwidth)) = L'#';       // Right Border
        }

        for(int j = 0; j < width; j++) {
            *(buf + (x + j) + (y * sn_screenwidth)) = L'#';                     // Top Border
            *(buf + (x + j) + ((y + height - 1) * sn_screenwidth)) = L'#';      // Bottom Border
        }
    }
}

void draw_str(wchar_t *buf, const wchar_t *str, int x, int y) {
    int str_len = wcslen(str);
    for(int i = 0; i < str_len; i++) {
        *(buf + (x + i) + (y * sn_screenwidth)) = *(str + i);
    }
}

void draw_button(wchar_t *buf, sButton * button) {
    draw_box(buf, button->px, button->py, button->width, button->height, false);
}
