#include <windows.h>
#include "widgets.h"
#include "const.h"
#include "input.h"
#include "tui.h"


void mouse_hover(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *mev) {
    if (mev->dwEventFlags == 0)
        a->state = HOVER;

    if (a == *old) return; 
    else {
        /* Checking if *old exists for the very first call of this function */
        if (*old) (*old)->state = NONE;
        a->state = HOVER;
        *old = a;
    }
}

void mouse_press(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *mev) {
    if (*old) (*old)->state = NONE;
    a->state = PRESS;
    *old = a;
    /* To make text cursor display properly */
    if (focused_wid) {
        switch (focused_wid->type) {
            case FIELD: focused_wid->widget.field.active = 0; break;

            default: break;
        }
    }
    focused_wid = a;
}

int check_disable(const sWidget *a) {
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
    a->widget.cbox.active = active;
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

    if (on == 0 && off == 1)               parent->active = 0; 
    if (on == 1 && off == 0)               parent->active = 1; 
    if ((on == 1 && off == 1) || mid == 1) parent->active = 2; 

    cbox_check_parent_active(a->widget.cbox.parent);
}

void frame_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *mev) {
    /* Basic stuff just for getting highlighting buttons to work properly */
    if (a->state == DISABLED || check_disable(a)) return;
    switch (mev->dwButtonState) {
        case FROM_LEFT_1ST_BUTTON_PRESSED:
            mouse_press(a, old, mev);
            break;
        default:
            mouse_hover(a, old, mev);
            break;
    }
}

void button_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *mev) {
    /* Not doing anything if the widget is disabled */
    if (a->state == DISABLED || check_disable(a)) return;

    switch (mev->dwButtonState) {
        case FROM_LEFT_1ST_BUTTON_PRESSED:
            /* If the state hasn't been changed since last time do nothing */
            if (a->state == PRESS || a->state != HOVER) {
                break;
            }

            if (a->widget.button.callback)
                a->widget.button.callback();
            mouse_press(a, old, mev);
            break;
        default:
            mouse_hover(a, old, mev);
            break;
    }
}

void label_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *mev) {
    /* Basic stuff just for getting highlighting buttons to work properly */
    if (a->state == DISABLED || check_disable(a)) return;
    switch (mev->dwButtonState) {
        case FROM_LEFT_1ST_BUTTON_PRESSED:
            mouse_press(a, old, mev);
            break;
        default:
            mouse_hover(a, old, mev);
            break;
    }
}

void cbox_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *mev) {
    /* Not doing anything if the widget is disabled */
    if (a->state == DISABLED || check_disable(a)) return;

    switch (mev->dwButtonState) {
        case FROM_LEFT_1ST_BUTTON_PRESSED:
            /* If the state hasn't been changed since last time do nothing */
            if (a->state == PRESS || a->state != HOVER) {
                break;
            }

            /* If the active variable is 2 it goes to 1 instead of 0 */
            if (a->widget.cbox.active == 2) a->widget.cbox.active = 1;
            else a->widget.cbox.active = !a->widget.cbox.active;
            cbox_set_active(a, a->widget.cbox.active);
            cbox_check_parent_active(a);
            mouse_press(a, old, mev);
            break;
        case 0:
            mouse_hover(a, old, mev);
            break;
        default:
            break;
    }
}

void rbutton_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *mev) {
    /* Not doing anything if the widget is disabled */
    if (a->state == DISABLED || check_disable(a)) return;

    switch (mev->dwButtonState) {
        case FROM_LEFT_1ST_BUTTON_PRESSED:
            /* If the state hasn't been changed since last time do nothing */
            if (a->state == PRESS || a->state != HOVER) {
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
            mouse_press(a, old, mev);
            break;
        default:
            mouse_hover(a, old, mev);
            break;
    }
}

void canvas_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *mev) {
    /* TODO: Implement passthrough of mevents to user */
    /* Basic stuff just for getting highlighting buttons to work properly */
    if (a->state == DISABLED || check_disable(a)) return;
    switch (mev->dwButtonState) {
        case FROM_LEFT_1ST_BUTTON_PRESSED:
            mouse_press(a, old, mev);
            if (a == focused_wid) a->widget.field.active = 1;
            break;
        default:
            mouse_hover(a, old, mev);
            break;
    }
}

void field_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *mev) {
    /* TODO: Let the mouse press set the cursor position */
    /* Basic stuff just for getting highlighting buttons to work properly */
    if (a->state == DISABLED || check_disable(a)) return;
    switch (mev->dwButtonState) {
        case FROM_LEFT_1ST_BUTTON_PRESSED:
            mouse_press(a, old, mev);
            if (focused_wid) a->widget.field.active = 1;
            break;
        default:
            mouse_hover(a, old, mev);
            break;
    }
}

void field_key_event(sWidget *a, const KEY_EVENT_RECORD *kev) {
    /* Return on key lift */
    if (kev->bKeyDown == 0) return;

    /* TODO: implement cursor movement */
    /* Normal keys */
    if (((kev->wVirtualKeyCode >= 0x30 && kev->wVirtualKeyCode <= 0x39) || // Numbers [0-9]
         (kev->wVirtualKeyCode >= 0x41 && kev->wVirtualKeyCode <= 0x5A) || // Letters [a-z]
         (kev->wVirtualKeyCode >= 0x60 && kev->wVirtualKeyCode <= 0x6F) || // Numpad
         (kev->wVirtualKeyCode >= 0xBA && kev->wVirtualKeyCode <= 0xC0) || // Other
         (kev->wVirtualKeyCode >= 0xDB && kev->wVirtualKeyCode <= 0xDF) || // Other
          kev->wVirtualKeyCode == 0x20                                  || // Space
          kev->wVirtualKeyCode == 0xE2)                                 && // Backslash
         a->widget.field.text.len < MAX_BUF_SIZE) {
        *(a->widget.field.text.text + a->widget.field.cursor.x) = kev->uChar.UnicodeChar;
        a->widget.field.cursor.x++;
        a->widget.field.text.len++;
    }
    /* Backspace */
    else if (kev->wVirtualKeyCode == 0x08 && 
       a->widget.field.cursor.x > 0) {
        *(a->widget.field.text.text + a->widget.field.cursor.x - 1) = L'\0';
        a->widget.field.cursor.x--;
        a->widget.field.text.len--;
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
                ;
                KEY_EVENT_RECORD *kev = &ir_inpbuf[i].Event.KeyEvent;
                if (!focused_wid) break;
                switch (focused_wid->type) {
                    case FIELD: field_key_event(focused_wid, kev); break;

                    default: break;
                }
                break;

            case MOUSE_EVENT:
                ;
                MOUSE_EVENT_RECORD *mev = &ir_inpbuf[i].Event.MouseEvent;
                /*
                 * Finding the widget
                 * Magic number picked for the size of the stack
                 */
                sStack *wid_search = create_stack(16);
                find_widget(wid_search, w_root, mev->dwMousePosition.X, mev->dwMousePosition.Y);
                sWidget *wid = stack_pop(wid_search);
                free(wid_search->arr);
                free(wid_search);
                /* Cases get handled in their own function to make the code more readable */
                switch (wid->type) {
                    case FRAME:       frame_mouse_event(wid, &old_wid, mev);   break;
                    case BUTTON:      button_mouse_event(wid, &old_wid, mev);  break;
                    case CHECKBOX:    cbox_mouse_event(wid, &old_wid, mev);    break;
                    case RADIOBUTTON: rbutton_mouse_event(wid, &old_wid, mev); break;
                    case LABEL:       label_mouse_event(wid, &old_wid, mev);   break;
                    case FIELD:       field_mouse_event(wid, &old_wid, mev);   break;

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
