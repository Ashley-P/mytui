/*
 * This is test sets up a frame and three buttons one on top of the other
 */

#include <stdio.h>
#include <windows.h>
#include <wchar.h>
#include "tui.h"



int main() {
    // Screen width and height for testing purposes
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);

    pWidget test_button1 = tui_button(w_root, L"TEST 1", NULL);
    pWidget test_button2 = tui_button(w_root, L"TEST 2", NULL);
    pWidget test_button3 = tui_button(w_root, L"TEST 3", NULL);

    grid_set(test_button1, 0, 0);
    grid_set(test_button2, 0, 1);
    grid_set(test_button3, 0, 2);

    tui_loop();
    return 0;
}
