#include <windows.h>
#include "input.h"
#include "tui.h"


void mouse_hover(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *ev) {
    if (ev->dwEventFlags == 0)
    a->state = HOVER;

    if (a == *old) return; 
    else {
        /* Checking if *old exists for the very first call of this function */
        if (*old) (*old)->state = NONE;
        a->state = HOVER;
        *old = a;
    }
}

int check_disable(sWidget *a) {
    sWidget *p = a->parent;
    while (p != NULL) {
        if (p->state == DISABLED) return 1;
        else p = p->parent;
    }
    return 0;
}

void cbox_set_active(sWidget *a, int active) {
    /* Travelling down the tree */
    for (int i = 0; i < a->widget.cbox.len; i++) cbox_set_active(a->widget.cbox.children[i], active);

    switch (active) {
        case 0:
            a->widget.cbox.active = 0;
            break;
        case 1:
            a->widget.cbox.active = 1;
            break;
        case 2:
            a->widget.cbox.active = 2;
            break;
        default:
            break;
    }
}

void cbox_check_parent_active(sWidget *a) {
    if (a->widget.cbox.parent == NULL) return;

    sCheckbox *parent = &a->widget.cbox.parent->widget.cbox;

    int on  = 0;
    int off = 0;
    int mid = 0;
    for (int i = 0; i < parent->len; i++) {
        switch (parent->children[i]->widget.cbox.active) {
            case 0:
                off = 1;
                break;
            case 1:
                on = 1;
                break;
            case 2:
                mid = 1;
                break;
            default:
                break;
        } 
    }

    if (on == 0 && off == 1)              parent->active = 0; 
    if (on == 1 && off == 0)              parent->active = 1; 
    if ((on == 1 && off == 1) || mid == 1) parent->active = 2; 

    cbox_check_parent_active(a->widget.cbox.parent);
}

void frame_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *ev) {
    /* Basic stuff just for getting highlighting buttons to work properly */
    if (a->state == DISABLED || check_disable(a)) return;
    mouse_hover(a, old, ev);

}

void button_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *ev) {
    /* Not doing anything if the widget is disabled */
    if (a->state == DISABLED || check_disable(a)) return;

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
            mouse_hover(a, old, ev);
            break;
        default:
            break;
    }
}

void cbox_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *ev) {
    /* Not doing anything if the widget is disabled */
    if (a->state == DISABLED || check_disable(a)) return;

    switch (ev->dwButtonState) {
        case FROM_LEFT_1ST_BUTTON_PRESSED:
            /* If the state hasn't been changed since last time do nothing */
            if (a->state == PRESS) {
                break;
            }

            a->state = PRESS;
            /* If the active variable is 2 it goes to 1 instead of 0 */
            if (a->widget.cbox.active == 2) a->widget.cbox.active = 1;
            else a->widget.cbox.active = !a->widget.cbox.active;
            cbox_set_active(a, a->widget.cbox.active);
            cbox_check_parent_active(a);
            *old = a;
            break;
        case 0:
            mouse_hover(a, old, ev);
            break;
        default:
            break;
    }
}

void rbutton_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *ev) {
    /* Not doing anything if the widget is disabled */
    if (a->state == DISABLED || check_disable(a)) return;

    switch (ev->dwButtonState) {
        case FROM_LEFT_1ST_BUTTON_PRESSED:
            /* If the state hasn't been changed since last time do nothing */
            if (a->state == PRESS) {
                break;
            }

            sRadiobuttonLink *p = a->widget.rbutton.parent;

            if (a->widget.rbutton.active == 0) {
                a->widget.rbutton.active = 1;
                for (int i = 0; i < p->len; i++) {
                    if (&a->widget.rbutton == p->children[i]) {
                        p->active = i;
                        break;
                    }
                }
                if (p->old) p->old->active = 0;
                p->old = &a->widget.rbutton;
            }
            break;
        case 0:
            mouse_hover(a, old, ev);
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
                    case FRAME:       frame_mouse_event(wid, &old_wid, ev);   break;
                    case BUTTON:      button_mouse_event(wid, &old_wid, ev);  break;
                    case CHECKBOX:    cbox_mouse_event(wid, &old_wid, ev);    break;
                    case RADIOBUTTON: rbutton_mouse_event(wid, &old_wid, ev); break;
                    case LABEL: break;

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
