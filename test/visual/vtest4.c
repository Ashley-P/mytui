/*
 * This test includes 9 frames each with 9 buttons set in a 3 x 3 grid with differently sized texts
 */

#include "tui.h"
#include "utils.h"

int main() {
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);
    pWidget frames[8];
    for(int i = 0; i < 9; i++) {
        frames[i] = tui_frame(w_root);

        for(int j = 0; j < 3; j++) {
            for(int k = 0; k < 3; k++) {
                grid_set(tui_button(frames[i], rand_str(), NULL), j, k); 
            }
        }
    }
    
    /* Some bug that doesn't set the size of the frame properly for the frames[0] */
    grid_set(frames[0], 0, 0);
    grid_set(frames[1], 0, 1);
    grid_set(frames[2], 0, 2);
    grid_set(frames[3], 1, 0);
    grid_set(frames[4], 1, 1);
    grid_set(frames[5], 1, 2);
    grid_set(frames[6], 2, 0);
    grid_set(frames[7], 2, 1);
    grid_set(frames[8], 2, 2);

    tui_loop();
    return 0;
}