#ifndef WIDGETS_H_
#define WIDGETS_H_

typedef struct Button_ {
    int px;
    int py;
    int width;
    int height;
    wchar_t *text;
} sButton, *psButton;


sButton * tui_button(const int px, const int py, const int width, const int height, wchar_t *text);

#endif
