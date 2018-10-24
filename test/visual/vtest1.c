/*
 * This test is for column and row spanning
 */

#include "tui.h"
#include "utils.h"

pWidget field;

void prnt_field() {
    tui_err(TUI_OTHER, 0, "Field: %ls", field->widget.field.text.text);
}

int main() {
    int n_screenwidth = 180;
    int n_screenheight = 50;

    tui_init(n_screenwidth, n_screenheight);
    
    pWidget label  = tui_label(w_root, L"Type something");
    field  = tui_field(w_root, L"In here", 20);
    pWidget field2  = tui_field(w_root, L"ANOTHER ONE", 60);
    pWidget button = tui_button(w_root, L"Send to error log more words", prnt_field);
    
    //field->psize.x = 1;
    label->usize.x = 10;

    button->usize.x = 8;
    button->widget.button.label.anchor = E;
    button->psize = (sSize) {1, 1};

    grid_set(label,  0, 0);
    grid_set(field,  0, 1);
    grid_set(field2, 0, 2);
    grid_set(button, 1, 1);

    //field->anchor = E;

    tui_loop();
    return 0;
}
