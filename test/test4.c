/*
 * This test includes 9 frames each with 9 buttons set in a 3 x 3 grid with differently sized texts
 */

#include "tui.h"
#include "utils.h"
#include <time.h>
#include <stdlib.h>

int main() {
    int n_screenwidth = 180;
    int n_screenheight = 50;

    /* random seed for text generation */
    srand(time(NULL));

    tui_init(n_screenwidth, n_screenheight);
    pWidget frames[1];
    for(int i = 0; i < 2; i++) {
        frames[i] = tui_frame(w_root);

        for(int j = 0; j < 3; j++) {
            for(int k = 0; k < 3; k++) {
                grid_set(tui_button(frames[i], L"test", NULL), j, k); 
            }
        }
    }

    grid_set(frames[0], 0, 0);
    grid_set(frames[1], 0, 1);
    /*
    grid_set(frames[2], 0, 2);
    grid_set(frames[3], 1, 0);
    grid_set(frames[4], 1, 1);
    grid_set(frames[5], 1, 2);
    grid_set(frames[6], 2, 0);
    grid_set(frames[7], 2, 1);
    grid_set(frames[8], 2, 2);
    */

    tui_loop();
    return 0;
}
