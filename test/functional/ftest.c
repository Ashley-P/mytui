/*
 * This test sets up some buttons in a 3 x 3 grid and tests the callback functionality
 */

#include "tui.h"
#include "utils.h"

void test1() { tui_err(TUI_OTHER, 0, "Button 1"); }
void test2() { tui_err(TUI_OTHER, 0, "Button 2"); }
void test3() { tui_err(TUI_OTHER, 0, "Button 3"); }
void test4() { tui_err(TUI_OTHER, 0, "Button 4"); }
void test5() { tui_err(TUI_OTHER, 0, "Button 5"); }
void test6() { tui_err(TUI_OTHER, 0, "Button 6"); }
void test7() { tui_err(TUI_OTHER, 0, "Button 7"); }
void test8() { tui_err(TUI_OTHER, 0, "Button 8"); }
void test9() { tui_err(TUI_OTHER, 0, "Button 9"); }

int main() {
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);
    
    pWidget myframe = tui_frame(w_root);
    grid_set(myframe, 0, 0);
    grid_set(tui_button(myframe, rand_str(), test1), 0, 0);
    grid_set(tui_button(myframe, rand_str(), test2), 1, 0);
    grid_set(tui_button(myframe, rand_str(), test3), 2, 0);
    grid_set(tui_button(myframe, rand_str(), test4), 0, 1);
    grid_set(tui_button(myframe, rand_str(), test5), 1, 1);
    grid_set(tui_button(myframe, rand_str(), test6), 2, 1);
    grid_set(tui_button(myframe, rand_str(), test7), 0, 2);
    grid_set(tui_button(myframe, rand_str(), test8), 1, 2);
    grid_set(tui_button(myframe, rand_str(), test9), 2, 2);

    tui_loop();
    return 0;
}
