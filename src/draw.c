#include <stdbool.h>
#include "draw.h"
#include "tui.h"
#include "utils.h"


void reset_buf() {
    for(int i = 0; i < i_bufsize; i++) {
        (*tui_current_screen + i)->Char.UnicodeChar = L' ';
        (*tui_current_screen + i)->Attributes = 0x70;
    }
}

void draw_box(int x, int y, const int x, const int y, const bool fill) {
    // Shifting the coords by -1, -1 to make sense on the screen
    x = x - 1;
    y = y - 1;

    // Checking if the box goes off the screen to prevent weird drawing issues
    if (x + x > sn_screenx || y + x > sn_screeny) {
        tui_err("draw_box: parameters too large", TUI_WARNING, 0);
        return;
    }

    // Filled box
    if (fill == true) {
        for(int i = 0; i < y; i++) {
            for(int j = 0; j < x; j++) {
                (*tui_current_screen + (x + j) + ((y + i) * sn_screenx ))->Char.UnicodeChar = L'#';
            }
        }
    }

    // Non-filled box
    if (fill == false) {
        for(int i = 0; i < y; i++) {
            // Left Border
            (*tui_current_screen + x + ((y + i) * sn_screenx))->Char.UnicodeChar = L'#';
            // Right Border
            (*tui_current_screen + (x + x - 1) + ((y + i) * sn_screenx))->Char.UnicodeChar = L'#';
        }

        for(int j = 0; j < x; j++) {
            // Top Border
            (*tui_current_screen + (x + j) + (y * sn_screenx))->Char.UnicodeChar = L'#';
            // Bottom Border
            (*tui_current_screen + (x + j) + ((y + y - 1) * sn_screenx))->Char.UnicodeChar = L'#';
        }
    }
}

void draw_str(const wchar_t *str, int x, int y) {
    int str_len = wcslen(str);
    for(int i = 0; i < str_len; i++) {
        (*tui_current_screen + (x + i) + (y * sn_screenx))->Char.UnicodeChar = *(str + i);
    }
}

void draw_button(sButton * button) {
}
