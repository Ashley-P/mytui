/*
 * This is test sets up a frame and three buttons one on top of the other
 */

#include <stdio.h>
#include <windows.h>
#include <wchar.h>
#include "tui.h"
#include "utils.h"

void myfunc1() {
    tui_err("test_button1", TUI_OTHER, 0);
}

void myfunc2() {
    tui_err("test_button2", TUI_OTHER, 0);
}

void myfunc3() {
    tui_err("test_button3", TUI_OTHER, 0);
}

int main() {
    // Screen x and y for testing purposes
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);

    pWidget test_button1 = tui_button(w_root, L"TEST 1", &myfunc1);
    pWidget test_button2 = tui_button(w_root, L"TEST 2", &myfunc2);
    pWidget test_button3 = tui_button(w_root, L"TEST 3", &myfunc3);

    grid_set(test_button1, 0, 0);
    grid_set(test_button2, 0, 1);
    grid_set(test_button3, 0, 2);

    tui_loop();
    return 0;
}
