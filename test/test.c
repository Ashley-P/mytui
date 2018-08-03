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

    pWidget test_frame = tui_frame(w_root);

    pWidget test_button1 = tui_button(test_frame, L"TEST 1", NULL);
    pWidget test_button2 = tui_button(test_frame, L"TEST 2", NULL);
    pWidget test_button3 = tui_button(test_frame, L"TEST 3", NULL);

    tui_loop();
    return 0;
}
