/*
 * This test is for column and row spanning
 */

#include "tui.h"
#include "utils.h"

int main() {
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);
    
    pWidget label1 = tui_label(w_root, L"label");
    pWidget label2 = tui_label(w_root, L"Padding");
    pWidget label3 = tui_label(w_root, L"label");

    grid_set(label1, 0, 0);
    grid_set(label2, 0, 1);
    grid_set(label3, 1, 0);

    label1->anchor = E;
    label3->anchor = W;
    label2->msize = (sSize) {3, 3};

    tui_loop();
    return 0;
}
