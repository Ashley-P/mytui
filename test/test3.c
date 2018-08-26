/*
 * This test includes 9 buttons in a 3 x 3 grid with differently sized texts
 */

#include "tui.h"

int main() {
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);
    
    /*
    grid_set(tui_button(w_root, L"asdasdfafds", NULL), 0, 0);
    grid_set(tui_button(w_root, L"oi", NULL), 0, 1);
    grid_set(tui_button(w_root, L"po340", NULL), 0, 2);
    grid_set(tui_button(w_root, L"test", NULL), 1, 0);
    grid_set(tui_button(w_root, L"yesboss", NULL), 1, 1);
    grid_set(tui_button(w_root, L"fae", NULL), 1, 2);
    grid_set(tui_button(w_root, L"interestingly cool", NULL), 2, 0);
    grid_set(tui_button(w_root, L"cooly interesting", NULL), 2, 1);
    grid_set(tui_button(w_root, L"memes", NULL), 2, 2);
    */

    grid_set(tui_button(w_root, rand_str(), NULL), 0, 0);
    grid_set(tui_button(w_root, rand_str(), NULL), 0, 1);
    grid_set(tui_button(w_root, rand_str(), NULL), 0, 2);
    grid_set(tui_button(w_root, rand_str(), NULL), 1, 0);
    grid_set(tui_button(w_root, rand_str(), NULL), 1, 1);
    grid_set(tui_button(w_root, rand_str(), NULL), 1, 2);
    grid_set(tui_button(w_root, rand_str(), NULL), 2, 0);
    grid_set(tui_button(w_root, rand_str(), NULL), 2, 1);
    grid_set(tui_button(w_root, rand_str(), NULL), 2, 2);

    tui_loop();
    return 0;
}
