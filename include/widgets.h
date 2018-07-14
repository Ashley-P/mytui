#ifndef WIDGETS_H_
#define WIDGETS_H_

typedef struct WidgetT {
    int type;
    union {
        sButton button;
        sFrame  frame;
    }
} sWidget;

typedef struct FrameT {
    int px;
    int py;
    int width;
    int height;
    sWidget children[16];
} sFrame;

typedef struct ButtonT {
    int px;
    int py;
    int width;
    int height;
    wchar_t *text;
    void (*draw)();     // Drawing function
    void (*callback)(); // Callback for the button
} sButton;


sButton * tui_button(const int px, const int py, const int width, const int height, wchar_t *text, void (*fp)());

#endif
