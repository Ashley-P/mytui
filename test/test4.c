/*
 * This test includes 9 frames each with 9 buttons set in a 3 x 3 grid with differently sized texts
 */

#include "tui.h"

int main() {
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);
    pFrame frames[9];
    for(int i = 0; i < 10; i++)
        frames[i] = tui_frame(w_root);


    tui_loop();
    return 0;
}
