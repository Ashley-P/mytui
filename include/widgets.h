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
    struct tWidget *parent;
    struct tWidget *children[16];
    union {
        struct tButton *button;
        struct tFrame  *frame;
    } widget;
} sWidget;

typedef struct tFrame {
    void (*draw)();     // Drawing function
} sFrame;

typedef struct tButton {
    wchar_t *text;
    void (*draw)();     // Drawing function
    void (*callback)(); // Callback for the button
} sButton;


sButton * tui_button(sFrame *parent, wchar_t *text, void (*callback)());

sFrame * tui_frame(sFrame *parent);

#endif
