/*
 * This test is for column and row spanning
 */

#include "tui.h"
#include "utils.h"

int main() {
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);
    
    pWidget label = tui_label(w_root, L"AAAAAAAAAAAAAAAAAAAAAAAAA");

    pWidget frame = tui_frame(w_root, L"");
    pWidget label2 = tui_label(frame, L"AAAAA");

    grid_set(frame, 0, 0);
    grid_set(label, 0, 1);
    grid_set(label2, 0, 0);

    tui_loop();
    return 0;
}
