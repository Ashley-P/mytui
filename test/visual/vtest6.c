/*
 * Extra testing for the anchor system
 */

#include "tui.h"

int main() {
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);

    pWidget test = tui_button(w_root, L"TEST", NULL);

    /* This is to get the button to stretch because padding isn't implemented yet */
    pWidget other = tui_button(w_root, L"AAAAAAAAAAAAAA", NULL);

    pWidget frame = tui_frame(w_root);
    pWidget but1  = tui_button(frame, L" ", NULL);
    pWidget but2  = tui_button(frame, L"^", NULL);
    pWidget but3  = tui_button(frame, L" ", NULL);
    pWidget but4  = tui_button(frame, L"<", NULL);
    pWidget but5  = tui_button(frame, L">", NULL);
    pWidget but6  = tui_button(frame, L" ", NULL);
    pWidget but7  = tui_button(frame, L"V", NULL);
    pWidget but8  = tui_button(frame, L" ", NULL);

    grid_set(test, 0, 0);
    grid_set(other, 0, 1);
    grid_set(frame, 1, 0);

    grid_set(but1, 0, 0);
    grid_set(but2, 1, 0);
    grid_set(but3, 2, 0);
    grid_set(but4, 0, 1);
    grid_set(but5, 2, 1);
    grid_set(but6, 0, 2);
    grid_set(but7, 1, 2);
    grid_set(but8, 2, 2);

    tui_loop();
}
