#ifndef WIDGETS_H_
#define WIDGETS_H_

typedef struct ButtonT {
    int px;
    int py;
    int width;
    int height;
    wchar_t *text;
    void (*draw)();     // Drawing function
    void (*callback)(); // Callback for the button
} sButton, *psButton;


sButton * tui_button(const int px, const int py, const int width, const int height, wchar_t *text, void (*fp)());

#endif
