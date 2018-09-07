/*
 * This test includes 9 buttons in a 3 x 3 grid with differently sized texts
 */

#include "tui.h"
#include "utils.h"

int main() {
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);
    
    pWidget myframe = tui_frame(w_root);
    grid_set(myframe, 0, 0);
    grid_set(tui_button(myframe, rand_str(), NULL), 0, 0);
    grid_set(tui_button(myframe, rand_str(), NULL), 0, 1);
    grid_set(tui_button(myframe, rand_str(), NULL), 0, 2);
    grid_set(tui_button(myframe, rand_str(), NULL), 1, 0);
    grid_set(tui_button(myframe, rand_str(), NULL), 1, 1);
    grid_set(tui_button(myframe, rand_str(), NULL), 1, 2);
    grid_set(tui_button(myframe, rand_str(), NULL), 2, 0);
    grid_set(tui_button(myframe, rand_str(), NULL), 2, 1);
    grid_set(tui_button(myframe, rand_str(), NULL), 2, 2);

    tui_loop();
    return 0;
}
