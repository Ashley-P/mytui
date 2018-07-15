#ifndef WIDGETS_H_
#define WIDGETS_H_

enum {
    FRAME  = 1,
    BUTTON = 2
};

typedef struct tWidget {
    int type;
    int px;
    int py;
    int width;
    int height;
    struct tFrame *parent;
    union {
        struct tButton *button;
        struct tFrame  *frame;
    } widget;
} sWidget;

typedef struct tFrame {
    sWidget *children[16];
} sFrame;

typedef struct tButton {
    wchar_t *text;
    void (*draw)();     // Drawing function
    void (*callback)(); // Callback for the button
} sButton;


sButton * tui_button(sFrame *parent, wchar_t *text, void (*callback)());

sFrame * tui_frame(sFrame *parent);

#endif
