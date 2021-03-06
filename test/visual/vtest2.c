/*
 * Extra testing for the anchor system
 */

#include "tui.h"

pWidget test;
pWidget frame;
pWidget frame2;

pRadiobuttonLink link;

void centre() {test->anchor = C;  redraw_widgets(w_root);}
void north()  {test->anchor = N;  redraw_widgets(w_root);}
void south()  {test->anchor = S;  redraw_widgets(w_root);}
void east()   {test->anchor = E;  redraw_widgets(w_root);}
void west()   {test->anchor = W;  redraw_widgets(w_root);}
void ne()     {test->anchor = N | E; redraw_widgets(w_root);}
void nw()     {test->anchor = N | W; redraw_widgets(w_root);}
void se()     {test->anchor = S | E; redraw_widgets(w_root);}
void sw()     {test->anchor = S | W; redraw_widgets(w_root);}
void ns()     {test->anchor = N | S; redraw_widgets(w_root);}
void ew()     {test->anchor = E | W; redraw_widgets(w_root);}
void nse()    {test->anchor = N | S | E; redraw_widgets(w_root);}
void nsw()    {test->anchor = N | S | W; redraw_widgets(w_root);}
void new()    {test->anchor = N | E | W; redraw_widgets(w_root);}
void sew()    {test->anchor = S | E | W; redraw_widgets(w_root);}
void nsew()   {test->anchor = N | S | E | W; redraw_widgets(w_root);}

void testrbutton() {tui_err(TUI_OTHER, 0, "x = %d, y = %d", w_root->rsize.x, w_root->rsize.y);} 

void disable_wid() {
    if (frame->state != DISABLED) {
        frame->state = DISABLED;
        frame2->state = DISABLED;
        test->state = DISABLED;
    } else {
        frame->state = NONE;
        frame2->state = NONE;
        test->state = NONE;
    }
}
        
int main() {
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);

    test  = tui_button(w_root, L"TEST1", testrbutton); 

    pWidget disable = tui_button(w_root, L"Disable", disable_wid);

    link = tui_radiobutton_link();

    pWidget label = tui_label(w_root, L"Label");

    pWidget cboxframe    = tui_frame(w_root, L"Checkbox With a really Long name");
    pWidget rbuttonframe = tui_frame(w_root, L"Radio Button");

    /* Canvas testing */
    pWidget frame3 = tui_frame(w_root, L"Canvas");
    pWidget canvas = tui_canvas(frame3, 20, 20);

    pWidget rbut1 = tui_radiobutton(rbuttonframe, L"Radio Button 1");
    pWidget rbut2 = tui_radiobutton(rbuttonframe, L"Radio Button 2");
    pWidget rbut3 = tui_radiobutton(rbuttonframe, L"Radio Button 3");
    pWidget rbut4 = tui_radiobutton(rbuttonframe, L"Radio Button 4");

    radiobutton_link(link, 4, rbut1, rbut2, rbut3, rbut4);

    pWidget cbox1 = tui_checkbox(cboxframe, L"Checkbox Test");
    pWidget cbox2 = tui_checkbox(cboxframe, L"Parent 1");
    pWidget cbox3 = tui_checkbox(cboxframe, L"Child 1");
    pWidget cbox4 = tui_checkbox(cboxframe, L"Child 2");
    pWidget cbox5 = tui_checkbox(cboxframe, L"Parent 2");
    pWidget cbox6 = tui_checkbox(cboxframe, L"Child 1");
    pWidget cbox7 = tui_checkbox(cboxframe, L"Child 2");

    frame = tui_frame(w_root, L"");
    pWidget but1  = tui_button(frame, L" ", nw);
    pWidget but2  = tui_button(frame, L"^", north);
    pWidget but3  = tui_button(frame, L" ", ne);
    pWidget but4  = tui_button(frame, L"<", west);
    pWidget but5  = tui_button(frame, L"C", centre);
    pWidget but6  = tui_button(frame, L">", east);
    pWidget but7  = tui_button(frame, L" ", sw);
    pWidget but8  = tui_button(frame, L"V", south);
    pWidget but9  = tui_button(frame, L" ", se);

    frame2 = tui_frame(w_root, L"Controls");
    pWidget but10  = tui_button(frame2, L"new", new);
    pWidget but11  = tui_button(frame2, L"nsw", nsw);
    pWidget but12  = tui_button(frame2, L"nsew", nsew);
    pWidget but13  = tui_button(frame2, L"nse", nse);
    pWidget but14  = tui_button(frame2, L"sew", sew);

    pWidget but15  = tui_button(frame2, L"ew", ew);
    pWidget but16  = tui_button(frame2, L"ns", ns);

    grid_set(test, 0, 0);
    grid_set(disable, 1, 1);
    grid_set(frame, 1, 0);
    grid_set(frame2, 2, 0);
    grid_set(frame3, 2, 2);
    grid_set(canvas, 0, 0);
    grid_set(label, 0, 1);
    grid_set(cboxframe, 0, 2);
    grid_set(rbuttonframe, 1, 2);

    grid_set(rbut1, 0, 0);
    grid_set(rbut2, 0, 1);
    grid_set(rbut3, 0, 2);
    grid_set(rbut4, 0, 3);

    grid_set(cbox1, 0, 0);
    grid_set(cbox2, 0, 1);
    grid_set(cbox3, 0, 2);
    grid_set(cbox4, 0, 3);
    grid_set(cbox5, 0, 4);
    grid_set(cbox6, 0, 5);
    grid_set(cbox7, 0, 6);

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

    grid_set(but15, 0, 0);
    grid_set(but16, 0, 2);

    frame2->widget.frame.label.anchor = NW;
    frame->anchor = NSEW;

    cboxframe->anchor = N;
    rbuttonframe->anchor = N;

    frame->msize.y = 5;
    frame->psize = (sSize) {1, 1};
    frame->bsize = (sSize) {2, 2};
    frame->anchor = E;

    frame2->bsize = (sSize) {1, 1};
    frame2->msize = (sSize) {2, 2};
    frame2->anchor = W;

    //test->msize = (sSize) {1, 1};
    //test->bsize = (sSize) {1, 1};
    //test->psize = (sSize) {1, 1};

    /* Testing out checkboxes */
    checkbox_add(cbox1, cbox2);
    checkbox_add(cbox1, cbox5);

    checkbox_add(cbox2, cbox3);
    checkbox_add(cbox2, cbox4);

    checkbox_add(cbox5, cbox6);
    checkbox_add(cbox5, cbox7);

    cbox2->anchor = W; 
    cbox5->anchor = W; 

    /* Testing radio buttons */
    rbut1->widget.rbutton.label.anchor = W;
    rbut2->widget.rbutton.label.anchor = W;
    rbut3->widget.rbutton.label.anchor = W;
    rbut4->widget.rbutton.label.anchor = W;

    /* Testing out Canvas */
    for (int i = 0; i < canvas->widget.canvas.len; i++) {
        (canvas->widget.canvas.canvas + i)->Attributes = (i % 0xF) * 16;
    }

    tui_loop();
}
