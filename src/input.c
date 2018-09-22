#include <windows.h>
#include "input.h"
#include "tui.h"



void frame_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *ev) {
    /* Basic stuff just for getting highlighting buttons to work properly */
    /* TODO: refactor this */
    if (a->state == DISABLED) return;
    if (a == *old) {
        return;
    } else {
        if (*old)
            (*old)->state = NONE;
        a->state = HOVER;
        *old = a;
    }

}

void button_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *ev) {
    /* Not doing anything if the widget is disabled */
    sWidget *p = a->parent;
    int isDisabled = 0;
    while (p != NULL) {
        if (p->state == DISABLED) {isDisabled = 1; return;}
        else {p = p->parent;}
    }
    if (a->state == DISABLED || isDisabled) return;

    switch (ev->dwButtonState) {
        case FROM_LEFT_1ST_BUTTON_PRESSED:
            /* If the state hasn't been changed since last time do nothing */
            if (a->state == PRESS) {
                break;
            }

            if (a->widget.button.callback)
                a->widget.button.callback();
            a->state = PRESS;
            *old = a;
            break;
        case 0:
            /* TODO: refactor this */
            if (ev->dwEventFlags == 0)
                a->state = HOVER;

            if (a == *old) {
                return;
            } else {
                /* for the first activation because you can't dereference a null pointer */
                if (*old)
                    (*old)->state = NONE;
                a->state = HOVER;
                *old = a;
            }
            break;
        default:
            break;
    }
}

void tui_handle_input() {
    static sWidget *old_wid = NULL; 
    unsigned long ul_evread;
    INPUT_RECORD ir_inpbuf[256];

    if (!ReadConsoleInput(h_stdin,
                          ir_inpbuf,
                          256,
                          &ul_evread))
        win_err("ReadConsoleInput");

    for(int i = 0; i < ul_evread; i++) {
        switch (ir_inpbuf[i].EventType) {
            case KEY_EVENT:
                tui_err(TUI_OTHER, 0, "Key Event");
                break;

            case MOUSE_EVENT:
                ;
                MOUSE_EVENT_RECORD *ev = &ir_inpbuf[i].Event.MouseEvent;
                /*
                 * Finding the widget
                 * Magic number picked for the size of the stack
                 */
                sStack *wid_search = create_stack(16);
                find_widget(wid_search, w_root, ev->dwMousePosition.X, ev->dwMousePosition.Y);
                sWidget *wid = stack_pop(wid_search);
                free(wid_search->arr);
                free(wid_search);
                /* Cases get handled in their own function to make the code more readable */
                switch(wid->type) {
                    case FRAME:  frame_mouse_event(wid, &old_wid, ev);  break;
                    case BUTTON: button_mouse_event(wid, &old_wid, ev); break;

                    default: break;
                }
                break;

            case WINDOW_BUFFER_SIZE_EVENT:
                tui_err(TUI_OTHER, 0, "Window Buffer Size Event");
                break;

            case FOCUS_EVENT: case MENU_EVENT: // Ignore these
                break;
        }
    }
}
