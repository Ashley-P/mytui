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
    if (x + width > sn_screenwidth || y + width > sn_screenheight)
        tui_err("draw_box: parameters too large", 0);

    // Filled box
    if (fill == true) {
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                *(buf + (x + i) + ((y + j) * sn_screenwidth )) = L'#';
            }
        }
    }

    // Non-filled box
    if (fill == false) {
        for(int i = 0; i < height; i++) {
            *(buf + (x + i) + (y * sn_screenwidth)) = L'#';                 // Top Border
            *(buf + (x + i) + ((y + height - 1) * sn_screenwidth)) = L'#';      // Bottom Border
            *(buf + x + ((y + i) * sn_screenwidth)) = L'#';                 // Left Border
            *(buf + (x + height - 1) + ((y + i) * sn_screenwidth)) = L'#';      // Right border
        }
    }
}

void draw_str(wchar_t *buf, const wchar_t *str, int x, int y) {
    int str_len = wcslen(str);
    for(int i = 0; i < str_len; i++) {
        *(buf + (x + i) + (y * sn_screenwidth)) = *(str + i);
    }
}
