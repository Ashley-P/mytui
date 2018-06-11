#ifndef WIDGETS_H_
#define WIDGETS_H_

typedef struct Button_ {
    int px;
    int py;
    int width;
    int height;
    char *text;
} sButton, *psButton;


psButton * tui_button(const int px, const int py, const int width, const int height, const char *text);
#endif
