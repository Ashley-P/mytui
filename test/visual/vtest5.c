/*
 * This test is for column and row spanning
 */

#include "tui.h"
#include "utils.h"

int main() {
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);

    pWidget but1 = tui_button(w_root, L"ROW SPAN", NULL);
    pWidget but2 = tui_button(w_root, L"COL SPAN", NULL);
    pWidget but3 = tui_button(w_root, rand_str(), NULL);
    pWidget but4 = tui_button(w_root, rand_str(), NULL);
    pWidget but5 = tui_button(w_root, rand_str(), NULL);
    pWidget but6 = tui_button(w_root, rand_str(), NULL);
    pWidget but7 = tui_button(w_root, L"COL SPAN 2", NULL);
    pWidget but8 = tui_button(w_root, L"ROW SPAN 2", NULL);


    /* Setting the spans */
    but1->rowspan = 3;
    but2->colspan = 2;
    but7->colspan = 4;
    but8->rowspan = 3;

    /* Wigdets to check */
    grid_set(but1, 0, 0);
    grid_set(but2, 1, 0);
    grid_set(but7, 0, 3);
    grid_set(but8, 3, 0);

    /* Other widgets for the above to work */
    grid_set(but3, 1, 1);
    grid_set(but4, 1, 2);
    grid_set(but5, 2, 1);
    grid_set(but6, 2, 2);
    
    tui_loop();
    return 0;
}
