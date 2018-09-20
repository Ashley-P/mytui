/*
 * Extra testing for the anchor system
 */

#include "tui.h"

pWidget test;

void centre() {test->anchor = C;  redraw_widgets(w_root);}
void north()  {test->anchor = N;  redraw_widgets(w_root);}
void south()  {test->anchor = S;  redraw_widgets(w_root);}
void east()   {test->anchor = E;  redraw_widgets(w_root);}
void west()   {test->anchor = W;  redraw_widgets(w_root);}
void ne()     {test->anchor = N | E; redraw_widgets(w_root);}
void nw()     {test->anchor = N | W; redraw_widgets(w_root);}
void se()     {test->anchor = S | E; redraw_widgets(w_root);}
void sw()     {test->anchor = S | W; redraw_widgets(w_root);}
void nse()    {test->anchor = N | S | E; redraw_widgets(w_root);}
void nsw()    {test->anchor = N | S | W; redraw_widgets(w_root);}
void new()    {test->anchor = N | E | W; redraw_widgets(w_root);}
void sew()    {test->anchor = S | E | W; redraw_widgets(w_root);}
void nsew()   {test->anchor = N | S | E | W; redraw_widgets(w_root);}

void testanchor() {tui_err(TUI_OTHER, 0, "Anchor == %d", test->anchor);}

int main() {
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);

    test = tui_button(w_root, L"TEST", testanchor); 

    /* This is to get the button to stretch because padding isn't implemented yet */
    pWidget other = tui_button(w_root, L"AAAAAAAAAAAAAA", NULL);

    pWidget frame = tui_frame(w_root);
    pWidget but1  = tui_button(frame, L" ", nw);
    pWidget but2  = tui_button(frame, L"^", north);
    pWidget but3  = tui_button(frame, L" ", ne);
    pWidget but4  = tui_button(frame, L"<", west);
    pWidget but5  = tui_button(frame, L"C", centre);
    pWidget but6  = tui_button(frame, L">", east);
    pWidget but7  = tui_button(frame, L" ", sw);
    pWidget but8  = tui_button(frame, L"V", south);
    pWidget but9  = tui_button(frame, L" ", se);

    pWidget frame2 = tui_frame(w_root);
    pWidget but10  = tui_button(frame2, L"new", new);
    pWidget but11  = tui_button(frame2, L"nsw", nsw);
    pWidget but12  = tui_button(frame2, L"nsew", nsew);
    pWidget but13  = tui_button(frame2, L"nse", nse);
    pWidget but14  = tui_button(frame2, L"sew", nsw);


    grid_set(test, 0, 0);
    grid_set(other, 0, 1);
    grid_set(frame, 1, 0);
    grid_set(frame2, 2, 0);

    grid_set(but1, 0, 0);
    grid_set(but2, 1, 0);
    grid_set(but3, 2, 0);
    grid_set(but4, 0, 1);
    grid_set(but5, 1, 1);
    grid_set(but6, 2, 1);
    grid_set(but7, 0, 2);
    grid_set(but8, 1, 2);
    grid_set(but9, 2, 2);

    grid_set(but10, 1, 0);
    grid_set(but11, 0, 1);
    grid_set(but12, 1, 1);
    grid_set(but13, 2, 1);
    grid_set(but14, 1, 2);

    tui_loop();
}
